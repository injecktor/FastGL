#include "gl_png.h"

void png_t::init(std::ofstream* file, unsigned int width, unsigned int height) {
    ASSERT(file != nullptr, "Trying to init empty file");
    m_file = file;
    add_png_structure();
    m_file = nullptr;
}

void png_t::add_png_structure() const {
    // for (const auto byte: m_png_structure_bytes) {
    //     *m_file << byte;
    // }
}

void png_t::add_ihdr_chunk() const {
    // for (const auto byte: m_png_ihdr_bytes) {
    //     *m_file << byte;
    // }
    // for (const auto byte: m_png_ihdr_data_bytes) {
    //     *m_file << byte;
    // }
}
