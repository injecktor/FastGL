#include "ppm.h"

#include <cmath>

ppm_t::ppm_t(ofstream *file, const unsigned width, const unsigned height) : m_file(file), m_width(width),
                                                                            m_height(height) {
}

void ppm_t::init() {
    *m_file << m_ppm_init << endl;
    *m_file << to_string(m_width) << ' ' << to_string(m_height) << endl;
    *m_file << "255" << endl; //rgb max
}

unsigned ppm_t::generate(const vector<color_t> &image_buffer) {
    for (auto pixel: image_buffer) {
        *m_file << to_string(pixel.getR()) << ' ' << to_string(pixel.getG()) << ' ' << to_string(pixel.getB()) <<
                endl;
    }
    return 0;
}

string ppm_t::get_format_extension() {
    return m_file_extension;
}
