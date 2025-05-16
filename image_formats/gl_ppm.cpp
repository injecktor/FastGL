#include "gl_ppm.hpp"

#include <cmath>

ppm_t::ppm_t(std::ofstream *file, const unsigned width, const unsigned height) 
    : m_file(file), m_width(width), m_height(height) {
}

void ppm_t::generate(const std::vector<color_t> &image_buffer, color_t background) {
    *m_file << m_identifier << std::endl;
    *m_file << std::to_string(m_width) << ' ' << std::to_string(m_height) << std::endl;
    *m_file << "255" << std::endl; //rgb max
    for (size_t i = 0; i < image_buffer.size(); ++i) {
        auto alphaed = color_t::alpha_to_color(image_buffer[i], background);
        *m_file << std::to_string(alphaed.r()) << ' ' << std::to_string(alphaed.g()) << ' ' 
        << std::to_string(alphaed.b()) << std::endl;
    }
}

std::string ppm_t::get_format_extension() {
    return m_file_extension;
}
