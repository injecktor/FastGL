#include "frame_process.h"

frame_process_t::frame_process_t(unsigned width, unsigned height) : m_width(width),
    m_height(height),
    m_resolution(width * height),
    m_background_color(color_t::white) {
    m_frame_buffer.reserve(m_resolution);
    m_background_bit_mask.reserve((m_resolution - 1) / 8 + 1);
    for (unsigned i = 0; i < m_resolution; ++i) {
        m_background_bit_mask.emplace_back(0xFF);
        m_frame_buffer.emplace_back(m_background_color);
    }
}

void frame_process_t::set_pixel(color_t color, point2_t point) {
    if (point.x >= m_width || point.y >= m_height) return;
    m_frame_buffer[point.y * m_width + point.x] = color;
    m_background_bit_mask[(point.y * m_width + point.x) / 8] &= 
        ~(1 << ((point.y * m_width + point.x) % 8));
}


void frame_process_t::set_background(color_t color) {
    for (unsigned i = 0; i < m_resolution; ++i) {
        if (m_background_bit_mask[i / 8] & (1 << (i % 8))) {
            m_frame_buffer[i] = color;
        }
    }
}

void frame_process_t::circle(color_t color, unsigned x, unsigned y, unsigned radius, bool fill) {
    ASSERT(radius != 0, "Can't draw circle with zero radius");
    const auto s_radius = static_cast<signed>(radius);
    for (signed i = -s_radius + 1; i <= s_radius - 1; ++i) {
        for (signed j = -s_radius + 1; j <= s_radius - 1; ++j) {
            if (is_in_circle(i, j, radius) && (fill || i == -s_radius + 1 || j == -s_radius + 1 || !
                                               is_in_circle(i - 1, j, radius) || !is_in_circle(i + 1, j, radius)
                                               || !is_in_circle(i, j - 1, radius) || !is_in_circle(i, j + 1, radius))) {
                set_pixel(color, { x + i, y + j });
            }
        }
    }
}

void frame_process_t::line(line_t line, point2_t start, point2_t end) {
    double x, y, tangent;
    unsigned last;
    bool along_x;
    const double dx = static_cast<signed>(end.x) - static_cast<signed>(start.x);
    const double dy = static_cast<signed>(end.y) - static_cast<signed>(start.y);
    if (abs(dx) > abs(dy)) {
        x = start.x;
        y = start.y;
        tangent = dy / dx;
        last = end.x;
        along_x = true;
    } else {
        x = start.y;
        y = start.x;
        tangent = dx / dy;
        last = end.y;
        along_x = false;
    }

    double upper_coord;
    double lower_coord;
    double upper_alpha;
    double upper_alpha;

    while (static_cast<unsigned>(x) <= last) {
        switch (line.antialiasing()) {
            case antialiasing_t::none: {
                upper_coord = round(y);
                lower_coord = round(y);  
            }
            break;
            case antialiasing_t::wu: {
                upper_coord = ceil(y);
                lower_coord = floor(y);    
            }
            break;
        }
        if (along_x) {
            set_pixel(color_t(line.color(), 1. - (upper_coord - y)), { static_cast<unsigned>(x), static_cast<unsigned>(upper_coord) });
            set_pixel(color_t(line.color(), 1. - (y - lower_coord)), { static_cast<unsigned>(x), static_cast<unsigned>(lower_coord) });
        } else {
            set_pixel(color_t(line.color(), 1. - (upper_coord - y)), { static_cast<unsigned>(upper_coord), static_cast<unsigned>(x) });
            set_pixel(color_t(line.color(), 1. - (y - lower_coord)), { static_cast<unsigned>(lower_coord), static_cast<unsigned>(x) });
        }
        y = y + tangent;
        x += 1;
    }
}

void frame_process_t::square(color_t color, unsigned width, unsigned x1, unsigned y1,
                             const unsigned length) {
    // line(color, width, x1, y1, x1 + length, y1);
    // line(color, width, x1, y1, x1, y1 + length);
    // line(color, width, x1 + length, y1, x1 + length, y1 + length);
    // line(color, width, x1, y1 + length, x1 + length, y1 + length);
}

void frame_process_t::rectangle(color_t color, unsigned width, unsigned x1, unsigned y1,
                                unsigned x2, unsigned y2) {
    // line(color, width, x1, y1, x2, y1);
    // line(color, width, x1, y1, x1, y2);
    // line(color, width, x2, y1, x2, y2);
    // line(color, width, x1, y2, x2, y2);
}

void frame_process_t::triangle(const color_t color, const unsigned width, const unsigned x1, const unsigned y1,
                               const unsigned x2, const unsigned y2, const unsigned x3,
                               const unsigned y3) {
    // line(color, width, x1, y1, x2, y2);
    // line(color, width, x3, y3, x2, y2);
    // line(color, width, x1, y1, x3, y3);
}

void frame_process_t::generate_image(const std::string &file_name, const image_type image_type) const {
    std::shared_ptr<image_format_t> img_gen;
    std::ofstream file;
    switch (image_type) {
        case image_type::ppm: {
            img_gen = std::make_shared<ppm_t>(&file, m_width, m_height);
        }
        break;
        case image_type::png: {
            ASSERT(false, "Not implemented");
        }
        break;
        default: {
            ASSERT(false, "Unknown image format");
        }
        break;
    }
    file.open("images/" + file_name + '.' + img_gen->get_format_extension(), 
        std::ofstream::out | std::ofstream::binary);
    ASSERT(file.is_open(), "Could not open file");
    img_gen->init();
    img_gen->generate(m_frame_buffer, m_background_color);
    file.close();
}

bool frame_process_t::is_in_circle(const signed x, const signed y, const unsigned radius) {
    if (pow(x, 2) + pow(y, 2) <= pow(radius, 2)) {
        return true;
    }
    return false;
}

void frame_process_t::apply_alpha() {
    for (size_t y = 0; y < m_height; y++) {
        for (size_t x = 0; x < m_width; x++) {
            set_pixel(color_t::apply_alpha(m_frame_buffer[y * m_width + x], m_background_color), { x, y });
        }
    }
}
