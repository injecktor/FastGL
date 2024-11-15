#include "ppm.h"

ppm_t::ppm_t(ofstream *file, unsigned width, unsigned height) : m_file(file), m_width(width + 100), m_height(height) {
    *m_file << m_ppm_init << endl;
    *m_file << to_string(m_width) << ' ' << to_string(m_height) << endl;
    *m_file << "255" << endl; //rgb max

    for (int i = 0; i < m_height; i++) {
        for (int j = 0; j < m_width; j++) {
            *m_file << 0 << ' ' << 0 << ' ' << 0 << endl;
        }
    }
}
