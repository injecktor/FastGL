#include "gl_png.hpp"

png_t::png_t(std::ofstream* file, unsigned width, unsigned height) 
    : m_file(file), m_width(width), m_height(height) {
    // unsigned char ihdr_bytes[21] = {
    //     // Data length
    //     0x00, 0x00, 0x00, 0x0d,
    //     // ascii "IHDR"
    //     0x49, 0x48, 0x44, 0x52,
    //     // Data
    //     0x00, 0x00, 0x00, 0x01, // Width
    //     0x00, 0x00, 0x00, 0x01, // Height
    //     0x08, // Bit Depth
    //     0x06, // Color Type
    //     0x00, // Compression Method
    //     0x00, // Filter Method
    //     0x00, // Interlace Method
    // };
    // unsigned long tmp_length = math_tools::htob32(*reinterpret_cast<unsigned long*>(ihdr_bytes));
    // auto tmp_length = *reinterpret_cast<unsigned long*>(ihdr_bytes);
    // m_ihdr = {tmp_length, "IHDR", m_ihdr_gen_data_bytes, };
}

void png_t::generate(const std::vector<color_t> &image_buffer, color_t background) {
    for (size_t i = 0; i < m_identifier.size(); i++) {
        *m_file << m_identifier[i];
    }

    // IHDR
    unsigned long crc = 0xffffffffUL;
    unsigned char ihdr_bytes[] = {
        // Data length
        0x00, 0x00, 0x00, 0x0d,
        // ascii "IHDR"
        0x49, 0x48, 0x44, 0x52,
        // Data
        0x00, 0x00, 0x00, 0x01, // Width
        0x00, 0x00, 0x00, 0x01, // Height
        0x08, // Bit Depth
        0x02, // Color Type
        0x00, // Compression Method
        0x00, // Filter Method
        0x00, // Interlace Method
    };
    const size_t ihdr_size = sizeof(ihdr_bytes);
    for (size_t i = 0; i < ihdr_size; i++) {
        *m_file << ihdr_bytes[i];
        if (i > 3) {
            crc = math_tools::crc32_fast(ihdr_bytes + i, 1, crc);
            crc ^= 0xffffffff;
        }
    }
    crc ^= 0xffffffff;
    unsigned char crc_0 = (crc & 0xff000000) >> 24;
    unsigned char crc_1 = (crc & 0xff0000) >> 16;
    unsigned char crc_2 = (crc & 0xff00) >> 8;
    unsigned char crc_3 = crc & 0xff;
    *m_file << crc_0;
    *m_file << crc_1;
    *m_file << crc_2;
    *m_file << crc_3;

    // IDAT
    crc = 0xffffffffUL;
    unsigned char ascii_idat[4] = { 0x49, 0x44, 0x41, 0x54 };
    const size_t image_buffer_size = image_buffer.size() * 3;
    unsigned char size_0 = (image_buffer_size & 0xff000000) >> 24;
    unsigned char size_1 = (image_buffer_size & 0xff0000) >> 16;
    unsigned char size_2 = (image_buffer_size & 0xff00) >> 8;
    unsigned char size_3 = image_buffer_size & 0xff;
    *m_file << size_0;
    *m_file << size_1;
    *m_file << size_2;
    *m_file << size_3;
    for (size_t i = 0; i < 4; i++) {
        *m_file << ascii_idat[i];
        crc = math_tools::crc32_fast(ascii_idat + i, 1, crc);
        crc ^= 0xffffffffUL;
    }
    for (size_t i = 0; i < image_buffer_size / 3; ++i) {
        auto alphaed = color_t::alpha_to_color(image_buffer[i], background);
        *m_file << (unsigned char)alphaed.r() << (unsigned char)alphaed.g()  
            << (unsigned char)(alphaed.b());
        crc = math_tools::crc32_fast(reinterpret_cast<unsigned char*>(&alphaed.r()), 1, crc);
        crc ^= 0xffffffffUL;
        crc = math_tools::crc32_fast(reinterpret_cast<unsigned char*>(&alphaed.g()), 1, crc);
        crc ^= 0xffffffffUL;
        crc = math_tools::crc32_fast(reinterpret_cast<unsigned char*>(&alphaed.b()), 1, crc);
        crc ^= 0xffffffffUL;
    }
    crc ^= 0xffffffffUL;
    crc_0 = (crc & 0xff000000) >> 24;
    crc_1 = (crc & 0xff0000) >> 16;
    crc_2 = (crc & 0xff00) >> 8;
    crc_3 = crc & 0xff;
    *m_file << crc_0;
    *m_file << crc_1;
    *m_file << crc_2;
    *m_file << crc_3;

    // IEND
    crc = 0xffffffffUL;
    *m_file << (unsigned char)0x00 << (unsigned char)0x00 << 
        (unsigned char)0x00 << (unsigned char)0x00;
    unsigned char ascii_iend[4] = { 0x49, 0x45, 0x4e, 0x44 };
    for (size_t i = 0; i < 4; i++) {
        *m_file << ascii_iend[i];
        crc = math_tools::crc32_fast(ascii_iend + i, 1, crc);
        crc ^= 0xffffffffUL;
    }
    crc ^= 0xffffffffUL;
    crc_0 = (crc & 0xff000000) >> 24;
    crc_1 = (crc & 0xff0000) >> 16;
    crc_2 = (crc & 0xff00) >> 8;
    crc_3 = crc & 0xff;
    *m_file << crc_0;
    *m_file << crc_1;
    *m_file << crc_2;
    *m_file << crc_3;
}

std::string png_t::get_format_extension() {
    return m_file_extension;
}