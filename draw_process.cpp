#include "draw_process.hpp"

draw_process_t::draw_process_t(unsigned width, unsigned height) : 
    m_width(width),
    m_height(height),
    m_resolution(width * height),
    m_background_color(color_t::white) {
    m_frame_buffer.reserve(m_resolution);
    m_background_bit_mask.reserve(((m_resolution - 1) >> 3) + 1);
    for (unsigned i = 0; i < m_resolution; ++i) {
        m_background_bit_mask.emplace_back(0xff);
        m_frame_buffer.emplace_back(m_background_color);
        m_last_color.emplace_back(0xff);
    }
}

void draw_process_t::set_pixel(color_t color, point2_t point, bool force) {
    if (point.x >= m_width || point.y >= m_height) return;
    auto index = point.y * m_width + point.x;
    if (color.a() == 0xff || force) {
        m_frame_buffer[index] = color;
        m_last_color[index] = color;
    } else {
        color_t new_color;
        auto prev_color = m_frame_buffer[index];
        auto prev_alpha = prev_color.get_alpha();
        auto alpha = color.get_alpha();
        auto alpha_reciprocal = 1. - alpha;
        new_color.set_alpha(1. - (1. - prev_alpha) * alpha_reciprocal);
        new_color.r() = alpha_reciprocal * prev_color.r() + alpha * color.r();
        new_color.g() = alpha_reciprocal * prev_color.g() + alpha * color.g();
        new_color.b() = alpha_reciprocal * prev_color.b() + alpha * color.b();
        m_frame_buffer[index] = new_color;
    }
    m_background_bit_mask[index >> 3] &= ~(1 << (index & 0b111));
}

void draw_process_t::clear_pixel(point2_t point) {
    auto index = point.y * m_width + point.x;
    m_frame_buffer[index] = m_background_color;
    m_background_bit_mask[index >> 3] |= 1 << (index & 0b111);
}

void draw_process_t::set_background(color_t color) {
    for (unsigned i = 0; i < m_resolution; ++i) {
        if (m_background_bit_mask[i >> 3] & (1 << (i & 0b111))) {
            m_frame_buffer[i] = color;
        }
    }
}

void draw_process_t::circle(color_t color, point2_t center, unsigned radius, bool fill) {
    ASSERT(radius != 0, "Can't draw circle with zero radius");
    unsigned radius_sqr = radius * radius;
    unsigned i_sqr = 0, i_sqr_prev = 0, i_sqr_next = 1;
    unsigned j_sqr = 0, j_sqr_prev = 0, j_sqr_next = 1;
    for (unsigned i = 0; i <= radius - 1; ++i) {
        for (unsigned j = 0; j <= radius - 1; ++j) {
            if (i_sqr + j_sqr < radius_sqr && (fill || i == 0 || j == 0
                                                || i_sqr_prev + j_sqr > radius_sqr || i_sqr_next + j_sqr > radius_sqr
                                                || i_sqr + j_sqr_prev > radius_sqr || i_sqr + j_sqr_next > radius_sqr)) {
                set_pixel(color, { center.x + i, center.y + j });
                set_pixel(color, { center.x - i, center.y + j });
                set_pixel(color, { center.x + i, center.y - j });
                set_pixel(color, { center.x - i, center.y - j });
                j_sqr_prev = j_sqr;
                j_sqr = j_sqr_next;
                j_sqr_next += (j << 1) + 3;
            } else {
                break;
            }
        }
        i_sqr_prev = i_sqr;
        i_sqr = i_sqr_next;
        i_sqr_next += (i << 1) + 3;
        j_sqr = 0; 
        j_sqr_prev = 0; 
        j_sqr_next = 1;
    }
}

void draw_process_t::line(line_t line, point2_t start, point2_t end) {
    double x, y, tangent;
    unsigned last;
    bool along_x, positive;
    const double dx = static_cast<signed>(end.x) - static_cast<signed>(start.x);
    const double dy = static_cast<signed>(end.y) - static_cast<signed>(start.y);
    double dl;
    unsigned length;
    if (abs(dx) > abs(dy)) {
        x = start.x;
        y = start.y;
        last = end.x;
        tangent = dy / dx;
        along_x = true;
        dl = dx;
    } else {
        x = start.y;
        y = start.x;
        last = end.y;
        tangent = dx / dy;
        along_x = false;
        dl = dy;
    }
    positive = dl > 0;
    dl = abs(dl) + 1;
    length = static_cast<unsigned>(static_cast<signed>(dl));

    double upper_coord;
    double lower_coord;
    double upper_alpha;
    double lower_alpha;

    auto aa = line.params().aa;
    auto color = line.color();
    auto alpha = line.color().get_alpha();

    unsigned counter = 0;
    while (counter < length) {
        switch (aa) {
            case line_antialiasing::none: {
                upper_coord = round(y);
                lower_coord = round(y);  
                upper_alpha = 1.;
                lower_alpha = 1.;
            }
            break;
            case line_antialiasing::wu: {
                upper_coord = ceil(y);
                lower_coord = floor(y);
                upper_alpha = 1. - (upper_coord - y);
                lower_alpha = 1. - (y - lower_coord);  
            }
            break;
        }
        if (along_x) {
            set_pixel(color_t(line.get_current_color(counter / dl), alpha * upper_alpha), 
                { static_cast<unsigned>(x), static_cast<unsigned>(upper_coord) });
            if (upper_coord != lower_coord) {
                set_pixel(color_t(line.get_current_color(counter / dl), alpha * lower_alpha), 
                    { static_cast<unsigned>(x), static_cast<unsigned>(lower_coord) });
            }
        } else {
            set_pixel(color_t(line.get_current_color(counter / dl), alpha * upper_alpha), 
                { static_cast<unsigned>(upper_coord), static_cast<unsigned>(x) });
            if (upper_coord != lower_coord) {
                set_pixel(color_t(line.get_current_color(counter / dl), alpha * lower_alpha), 
                    { static_cast<unsigned>(lower_coord), static_cast<unsigned>(x) });
            }
        }
        y = y + tangent;
        x = positive ? ++x : --x;
        counter++;
    };
}

void draw_process_t::square(line_t line, point2_t point, unsigned length, bool fill) {
    draw_process_t::line(line, { point.x, point.y }, { point.x + length - 1, point.y });
    draw_process_t::line(line, { point.x, point.y + 1 }, { point.x, point.y + length - 2 });
    draw_process_t::line(line, { point.x + length - 1, point.y + 1 }, { point.x + length - 1, point.y + length - 2 });
    draw_process_t::line(line, { point.x, point.y + length - 1 }, { point.x + length - 1, point.y + length  - 1 });
    if (fill) {
        for (unsigned i = point.x + 1; i < point.x + length - 1; i++) {
            for (unsigned j = point.y + 1; j < point.y + length - 1; j++) {
                set_pixel(line.color(), { i, j } );
            }
        }
    }
}

void draw_process_t::rectangle(color_t color, unsigned width, unsigned x1, unsigned y1,
                                unsigned x2, unsigned y2) {
    // line(color, width, x1, y1, x2, y1);
    // line(color, width, x1, y1, x1, y2);
    // line(color, width, x2, y1, x2, y2);
    // line(color, width, x1, y2, x2, y2);
}

void draw_process_t::triangle(const color_t color, const unsigned width, const unsigned x1, const unsigned y1,
                               const unsigned x2, const unsigned y2, const unsigned x3,
                               const unsigned y3) {
    // line(color, width, x1, y1, x2, y2);
    // line(color, width, x3, y3, x2, y2);
    // line(color, width, x1, y1, x3, y3);
}

void draw_process_t::generate_image(const std::string &file_name, const image_type image_type) const {
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
    img_gen->generate(m_frame_buffer, m_last_color);
    file.close();
}

bool draw_process_t::is_in_circle(const signed x, const signed y, const unsigned radius) {
    if (pow(x, 2) + pow(y, 2) <= pow(radius, 2)) {
        return true;
    }
    return false;
}

void draw_process_t::alpha_to_color() {
    for (unsigned y = 0; y < m_height; y++) {
        for (unsigned x = 0; x < m_width; x++) {
            set_pixel(color_t::alpha_to_color(m_frame_buffer[y * m_width + x], m_background_color), { x, y });
        }
    }
}
