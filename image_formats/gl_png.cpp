#include "gl_png.hpp"

png_t::png_t(std::ofstream* file, unsigned width, unsigned height) 
    : m_file(file), m_width(width), m_height(height) {
}

void png_t::generate(const std::vector<color_t> &image_buffer, color_t background) {
    for (size_t i = 0; i < image_buffer.size(); ++i) {
        auto alphaed = color_t::alpha_to_color(image_buffer[i], background);
        *m_file << std::to_string(alphaed.r()) << ' ' << std::to_string(alphaed.g()) << ' ' 
        << std::to_string(alphaed.b()) << std::endl;
    }
}

std::string png_t::get_format_extension() {
    return m_file_extension;
}