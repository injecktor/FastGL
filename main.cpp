#include <iostream>
#include <fstream>
#include <cstdint>

#include "logging.h"

const uint8_t png_start_bytes[] = {
    0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A, 0x00, 0x00, 0x00, 0x0D, 0x49, 0x48, 0x44, 0x52
};

const uint8_t png_size_bytes[] = {
    0x00, 0x00, 0x00, 0x64, 0x00, 0x00, 0x00, 0x64
};

using namespace std;

int main() {
    ofstream fout("../image.png", ios::out | ios::binary);

    ASSERT(fout.is_open(), "Error opening file");

    for (const auto byte : png_start_bytes) {
        fout << byte;
    }

    for (const auto byte : png_size_bytes) {
        fout << byte;
    }

    fout.close();

    return 0;
}
