#ifndef GL_PNG_H
#define GL_PNG_H

#include <iostream>
#include <fstream>
#include <cstdint>
#include <array>

#include "color.hpp"
#include "image_format.hpp"

class png_t : public image_format_t {
    struct chunk_t {
        unsigned length;
        unsigned type;
        char* data;
        unsigned crc;
    };
public:
    png_t(std::ofstream *file, unsigned width, unsigned height);
    void generate(const std::vector<color_t> &image_buffer, color_t background) override;
    std::string get_format_extension() override;

private:
    std::ofstream *m_file = nullptr;
    std::string m_file_extension = "png";
    unsigned m_width;
    unsigned m_height;

    const std::array<const int, 8> m_identifier = {
        0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A
    };
    //
    const std::array<const int, 8> m_ihdr_bytes = {
        0x00, 0x00, 0x00, 0x0D, 0x49, 0x48, 0x44, 0x52
    };
    const std::array<const int, 13> m_ihdr_gen_data_bytes = {
        0x00, 0x00, 0x00, 0x01, // Width
        0x00, 0x00, 0x00, 0x01, // Height
        0x08, // Bit Depth
        0x06, // Color Type
        0x00, // Compression Method
        0x00, // Filter Method
        0x00, // Interlace Method
    };
};


#endif //GL_PNG_H
