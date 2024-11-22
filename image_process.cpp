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
    if (x >= m_width || y >= m_height) return;
    m_image_buffer[y * m_width + x] = color;
    m_background_bit_mask[(y * m_width + x) / 8] &= ~(1 << ((y * m_width + x) % 8));
}


void image_process_t::set_background(const color_t color) {
    for (unsigned i = 0; i < m_resolution; ++i) {
        if (m_background_bit_mask[i / 8] & (1 << (i % 8))) {
            m_image_buffer[i] = color;
        }
    }
}

void image_process_t::circle(const color_t color, const unsigned x, const unsigned y, const unsigned radius,
                             bool fill) {
    const auto s_radius = static_cast<signed>(radius);
    ASSERT(radius != 0, "Can't draw circle with zero radius");
    for (signed i = -s_radius + 1; i <= s_radius - 1; ++i) {
        for (signed j = -s_radius + abs(i) + 1; j <= s_radius - abs(i) - 1; ++j) {
            if (fill || j == -s_radius + abs(i) + 1 || j == s_radius - abs(i) - 1) {
                set_pixel(color, x + i, y + j);
            }
        }
    }
}

void image_process_t::line(const color_t color, const unsigned width, const unsigned x1, const unsigned y1,
                           const unsigned x2, const unsigned y2) {
    double x = x1, y = y1;
    const double dx = static_cast<signed>(x2 - x1);
    const double dy = static_cast<signed>(y2 - y1);
    if (abs(dx) > abs(dy)) {
        while (static_cast<unsigned>(x) != x2) {
            circle(color, static_cast<unsigned>(x), static_cast<unsigned>(round(y)), width);
            y = y + dy / dx;
            x += 1;
        }
    } else {
        while (static_cast<unsigned>(y) != y2) {
            circle(color, static_cast<unsigned>(round(x)), static_cast<unsigned>(y), width);
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

void image_process_t::rectangle(const color_t color, const unsigned width, const unsigned x1, const unsigned y1,
                                const unsigned x2, const unsigned y2) {
    line(color, width, x1, y1, x2, y1);
    line(color, width, x1, y1, x1, y2);
    line(color, width, x2, y1, x2, y2);
    line(color, width, x1, y2, x2, y2);
}

void image_process_t::triangle(const color_t color, const unsigned width, const unsigned x1, const unsigned y1,
                               const unsigned x2, const unsigned y2, const unsigned x3,
                               const unsigned y3) {
    line(color, width, x1, y1, x2, y2);
    line(color, width, x3, y3, x2, y2);
    line(color, width, x1, y1, x3, y3);
}

void image_process_t::generate_image(const string &file_name, const image_type image_type) const {
    image_generator_t *i_g;
    ofstream file;
    switch (image_type) {
        case ppm: {
            i_g = new ppm_t(&file, m_width, m_height);
        }
        break;
        case png: {
            ASSERT(false, "Not implemented");
        }
        break;
        default: {
            ASSERT(false, "Unknown image format");
        }
        break;
    }
    file.open("../" + file_name + '.' + i_g->get_format_extension(), ofstream::out | ofstream::binary);
    ASSERT(file.is_open(), "Could not open file");
    i_g->init();
    i_g->generate(m_image_buffer);
    file.close();
    delete i_g;
}
