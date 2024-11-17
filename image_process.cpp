#include "image_process.h"

image_process_t::image_process_t(const unsigned width, const unsigned height) : m_width(width),
    m_height(height),
    m_resolution(width * height) {
    m_image_buffer.reserve(m_resolution);
    m_background_bit_mask.reserve((m_resolution - 1) / 8 + 1);
    for (unsigned i = 0; i < m_resolution; ++i) {
        m_background_bit_mask.emplace_back(0xFF);
        m_image_buffer.emplace_back(color_t::black);
    }
}

void image_process_t::set_pixel(const color_t color, const unsigned x, const unsigned y) {
    m_image_buffer[y * m_width + x] = color;
    m_background_bit_mask[(y * m_width + x) / 8] &= ~(1 << ((y * m_width + x) % 8));
}


void image_process_t::set_background(const color_t color) {
    for (unsigned i = 0; i < m_resolution; ++i) {
        if (m_background_bit_mask[i]) {
            m_image_buffer[i] = color;
        }
    }
}

void image_process_t::line(const color_t color, const unsigned width, const unsigned x1, const unsigned y1,
                           const unsigned x2, const unsigned y2) {
    double x = x1, y = y1;
    const double dx = x2 - x1;
    const double dy = y2 - y1;
    if (abs(dx) > abs(dy)) {
        while (static_cast<unsigned>(x) != x2) {
            for (unsigned i = 0; i < width; ++i) {
                set_pixel(color, static_cast<unsigned>(x), static_cast<unsigned>(round(y - (width - 1) / 2. + i)));
            }
            y = y + dy / dx;
            x += 1;
        }
    } else {
        while (static_cast<unsigned>(y) != y2) {
            for (unsigned i = 0; i < width; ++i) {
                set_pixel(color, static_cast<unsigned>(round(x - (width - 1) / 2. + i)), static_cast<unsigned>(y));
            }
            x = x + dx / dy;
            y += 1;
        }
    }
}

void image_process_t::square(const color_t color, const unsigned width, const unsigned x1, const unsigned y1,
                             const unsigned length) {
    line(color, width, x1, y1, x1 + length, y1);
    line(color, width, x1, y1, x1, y1 + length);
    line(color, width, x1 + length, y1, x1 + length, y1 + length);
    line(color, width, x1, y1 + length, x1 + length, y1 + length);
}

void image_process_t::generate_image(const string& file_name, const image_type image_type) const {
    image_generator_t* i_g;
    ofstream file;
    switch (image_type) {
        case ppm: {
            i_g = new ppm_t(&file, m_width, m_height);
        } break;
        case png: {
            ASSERT(false, "Not implemented");
        } break;
        default: {
            ASSERT(false, "Unknown image format");
        } break;
    }
    file.open("../" + file_name + '.' + i_g->get_format_extension(), ofstream::out | ofstream::binary);
    ASSERT(file.is_open(), "Could not open file");
    i_g->init();
    i_g->generate(m_image_buffer);
    file.close();
    delete i_g;
}
