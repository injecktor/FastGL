#include "draw_process.hpp"

draw_process_t::draw_process_t(signed width, signed height) : 
    m_width(width),
    m_height(height),
    m_resolution(width * height),
    m_background_color(color_t::white) {
    m_frame_buffer.reserve(m_resolution);
    m_flags.reserve(m_resolution);
    for (unsigned i = 0; i < m_resolution; ++i) {
        m_flags.emplace_back(1);
        m_frame_buffer.emplace_back(m_background_color);
    }
}

void draw_process_t::set_pixel(color_t color, point2_t point, bool force, bool mark_pixel) {
    if (point.x >= m_width || point.y >= m_height) return;
    if (point.x < 0 || point.y < 0) return;
    auto index = point.y * m_width + point.x;
    if (color.a() == 0xff || check_flag(flag_t::background, index) || force) {
        m_frame_buffer[index] = color;
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
    set_flag(flag_t::background, index, false);
    if (mark_pixel) {
        set_flag(flag_t::current, index, true);
    }
}

color_t draw_process_t::get_pixel(point2_t point) {
    return m_frame_buffer[point.y * m_width + point.x];
}

void draw_process_t::clear_pixel(point2_t point) {
    if (point.x >= m_width || point.y >= m_height) return;
    auto index = point.y * m_width + point.x;
    m_frame_buffer[index] = m_background_color;
    set_flag(flag_t::background, index, true);
}

void draw_process_t::set_background(color_t color) {
    for (unsigned i = 0; i < m_resolution; ++i) {
        if (check_flag(flag_t::background, i)) {
            m_frame_buffer[i] = color;
        }
    }
}

void draw_process_t::clear() {
    for (unsigned i = 0; i < m_resolution; ++i) {
        set_flag(flag_t::background, i, true);
        m_frame_buffer[i] = m_background_color;
    }
}

void draw_process_t::circle(color_t color, point2_t center, unsigned radius, bool fill) {
    ASSERT(radius != 0, "Can't draw circle with zero radius");
    unsigned radius_sqr = radius * radius;
    unsigned i_sqr = 0, i_sqr_prev = 0, i_sqr_next = 1;
    unsigned j_sqr = 0, j_sqr_prev = 0, j_sqr_next = 1;
    for (signed i = 0; i <= radius - 1; ++i) {
        for (signed j = 0; j <= radius - 1; ++j) {
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

void draw_process_t::line(line_t line, point2_t start, point2_t end, bool mark_pixel) {
    double x, y, tangent;
    unsigned last;
    bool along_x, positive;
    const double dx = end.x - start.x;
    const double dy = end.y - start.y;
    double dl;
    unsigned length;
    auto abs_dx = abs(dx);
    auto abs_dy = abs(dy);
    if (abs_dx > abs_dy) {
        x = start.x;
        y = start.y;
        last = end.x;
        tangent = dy / abs_dx;
        along_x = true;
        dl = dx;
    } else {
        x = start.y;
        y = start.x;
        last = end.y;
        tangent = dx / abs_dy;
        along_x = false;
        dl = dy;
    }
    positive = dl > 0;
    dl = abs(dl) + 1;
    length = static_cast<unsigned>(static_cast<signed>(dl));

    double upper_coord, lower_coord, upper_alpha, lower_alpha;

    auto aa = line.params().aa;
    auto color = line.color();
    auto alpha = color.get_alpha();

    unsigned counter = 0;
    double step = 1 / dl, ratio = 0;
    while (counter < length) {
        switch (aa) {
            case line_antialiasing::none: {
                upper_coord = round(y);
                lower_coord = upper_coord;  
                upper_alpha = 1.;
                lower_alpha = 1.;
            }
            break;
            case line_antialiasing::wu: {
                upper_coord = ceil(y);
                lower_coord = upper_coord - 1;
                upper_alpha = 1. - (upper_coord - y);
                lower_alpha = 1. - (y - lower_coord);  
            }
            break;
        }
        color_t current_color = line.get_current_color(ratio);
        if (along_x) {
            set_pixel(color_t(current_color, alpha * upper_alpha), 
                { static_cast<signed>(x), static_cast<signed>(upper_coord) }, false, mark_pixel);
            if (upper_coord != lower_coord) {
                set_pixel(color_t(current_color, alpha * lower_alpha), 
                    { static_cast<signed>(x), static_cast<signed>(lower_coord) }, false, mark_pixel);
            }
        } else {
            set_pixel(color_t(current_color, alpha * upper_alpha), 
                { static_cast<signed>(upper_coord), static_cast<signed>(x) }, false, mark_pixel);
            if (upper_coord != lower_coord) {
                set_pixel(color_t(current_color, alpha * lower_alpha), 
                    { static_cast<signed>(lower_coord), static_cast<signed>(x) }, false, mark_pixel);
            }
        }
        y = y + tangent;
        x = positive ? ++x : --x;
        ratio += step;
        counter++;
    };
}

void draw_process_t::rectangle(line_t line, point2_t point, unsigned width, unsigned height, bool fill, rect_params_t rect_params) {
    auto sina = sin(rect_params.rotation);
    auto cosa = cos(rect_params.rotation);
    width--;
    height--;
    point2_t point2(point.x + cosa * width, point.y + sina * width);
    point2_t point3(point.x - sina * height, point.y + cosa * height);
    point2_t point4(point2.x + point3.x - point.x, point2.y + point3.y - point.y);
    draw_process_t::line(line, { point.x, point.y }, { point2.x, point2.y });
    draw_process_t::line(line, { point.x, point.y }, { point3.x, point3.y });
    draw_process_t::line(line, { point4.x, point4.y }, { point2.x, point2.y });
    draw_process_t::line(line, { point4.x, point4.y }, { point3.x, point3.y });
    if (fill) {
        for (signed i = point.x + 1; i < point.x + width - 1; i++) {
            for (signed j = point.y + 1; j < point.y + height - 1; j++) {
                set_pixel(line.color(), { i, j } );
            }
        }
    }
}

void draw_process_t::triangle(line_t line, point2_t point1, point2_t point2, point2_t point3, bool fill) {
    draw_process_t::line(line, point1, point2);
    draw_process_t::line(line, point1, point3);
    draw_process_t::line(line, point2, point3);
}

void draw_process_t::generate_image(const std::string &file_name, image_type_t image_type) const {
    std::shared_ptr<image_format_t> img_gen;
    std::ofstream file;
    switch (image_type) {
        case image_type_t::ppm: {
            img_gen = std::make_shared<ppm_t>(&file, m_width, m_height);
        }
        break;
        case image_type_t::png: {
            ASSERT(false, "Not implemented");
        }
        break;
        default: {
            ASSERT(false, "Unknown image format");
        }
        break;
    }
    file.open("images/" + file_name, std::ofstream::out | std::ofstream::binary);
    ASSERT(file.is_open(), "Could not open file");
    img_gen->init();
    img_gen->generate(m_frame_buffer, m_background_color);
    file.close();
}

void draw_process_t::render() {
}

const std::vector<color_t>& draw_process_t::get_frame_buffer() {
    return m_frame_buffer;
}

inline bool draw_process_t::check_flag(flag_t flag, unsigned index) {
    return m_flags[index] & (1 << flag);
}

inline void draw_process_t::set_flag(flag_t flag, unsigned index, bool value) {
    if (value) {
        m_flags[index] |= (1 << flag);
    } else {
        m_flags[index] &= ~(1 << flag);
    }
}

void draw_process_t::alpha_to_color() {
    for (signed y = 0; y < m_height; y++) {
        for (signed x = 0; x < m_width; x++) {
            set_pixel(color_t::alpha_to_color(m_frame_buffer[y * m_width + x], m_background_color), { x, y });
        }
    }
}
