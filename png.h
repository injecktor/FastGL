#ifndef PNG_INIT_H
#define PNG_INIT_H

#include <iostream>
#include <fstream>
#include <cstdint>

#include "logging.h"

using namespace std;

class png_t {
public:
    void init(ofstream *file, unsigned int width, unsigned int height);

private:
    ofstream *m_file = nullptr;

    // const uint8_t m_png_structure_bytes[] = {
    //     0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A
    // };
    //
    // const uint8_t m_png_ihdr_bytes[] = {
    //     0x00, 0x00, 0x00, 0x0D, 0x49, 0x48, 0x44, 0x52
    // };
    // const uint8_t m_png_ihdr_data_bytes[] = {
    //     0x00, 0x00, 0x00, 0x01, // Width
    //     0x00, 0x00, 0x00, 0x01, // Height
    //     0x01, // Bit Depth
    //     0x00, // Color Type
    //     0x00, // Compression Method
    //     0x00, // Filter Method
    //     0x00, // Interlace Method
    // };

    void add_png_structure() const;

    void add_ihdr_chunk() const;
};


#endif //PNG_INIT_H
