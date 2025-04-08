#include "cge_ppm.h"

#include <cmath>

ppm_t::ppm_t(std::ofstream *file, const unsigned width, const unsigned height) : m_file(file), m_width(width),
                                                                            m_height(height) {
}

void ppm_t::init() {
    *m_file << m_ppm_init << std::endl;
    *m_file << std::to_string(m_width) << ' ' << std::to_string(m_height) << std::endl;
    *m_file << "255" << std::endl; //rgb max
}

unsigned ppm_t::generate(const std::vector<color_t> &image_buffer) {
    for (auto pixel: image_buffer) {
        *m_file << std::to_string(pixel.get_r()) << ' ' << std::to_string(pixel.get_g()) << ' ' 
        << std::to_string(pixel.get_b()) << std::endl;
    }
    return 0;
}

std::string ppm_t::get_format_extension() {
    return m_file_extension;
}
