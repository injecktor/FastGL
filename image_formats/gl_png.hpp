#ifndef GL_PNG_H
#define GL_PNG_H

#include <iostream>
#include <fstream>
#include <cstdint>
#include <array>
#include <cstring>

#include "color.hpp"
#include "image_format.hpp"

class png_t : public image_format_t {
    struct chunk_t {
        unsigned length;
        char type[5];
        unsigned char* data;
        unsigned crc;
    };
public:
    png_t(std::ifstream *file);
    png_t(std::ofstream *file, unsigned width, unsigned height);
    void generate(const std::vector<color_t> &image_buffer, color_t background) override;
    std::string get_format_extension() override;

private:
    std::ofstream *m_file = nullptr;
    std::string m_file_extension = "png";
    unsigned m_width;
    unsigned m_height;


    const std::array<unsigned char, 8> m_identifier = {
        0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A
    };

    chunk_t m_ihdr;
    chunk_t m_idat;
    chunk_t m_iend;
};


#endif //GL_PNG_H
