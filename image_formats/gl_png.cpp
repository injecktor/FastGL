#include "gl_png.hpp"

png_t::png_t(std::ofstream* file, unsigned width, unsigned height) 
    : m_file(file), m_width(width), m_height(height) {
    unsigned char ihdr_bytes[21] = {
        // Data length
        0x00, 0x00, 0x00, 0x0d,
        // ascii "IHDR"
        0x49, 0x48, 0x44, 0x52,
        // Data
        0x00, 0x00, 0x00, 0x01, // Width
        0x00, 0x00, 0x00, 0x01, // Height
        0x08, // Bit Depth
        0x06, // Color Type
        0x00, // Compression Method
        0x00, // Filter Method
        0x00, // Interlace Method
    };
    auto tmp_length = swapEndian32(*reinterpret_cast<unsigned long*>(ihdr_bytes));
    auto tmp_length = *reinterpret_cast<unsigned long*>(ihdr_bytes);
    m_ihdr = {tmp_length, "IHDR", m_ihdr_gen_data_bytes, };
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