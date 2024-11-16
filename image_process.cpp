#include "image_process.h"

image_process_t::image_process_t(const unsigned width, const unsigned height) : m_width(width),
    m_height(height),
    m_resolution(width * height) {
    m_image_buffer.reserve(m_resolution);
    m_background_bit_mask.reserve(m_resolution);
    for (unsigned i = 0; i < m_resolution; ++i) {
        m_background_bit_mask.emplace_back(1);
        m_image_buffer.emplace_back(color_t::black);
    }
}

void image_process_t::set_pixel(const color_t color, const unsigned x, const unsigned y) {
    m_image_buffer[y * m_width + x] = color;
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
