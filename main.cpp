#include <iostream>
#include <fstream>
#include <cstdint>

#include "logging.h"
#include "ppm.h"

using namespace std;

int main() {
    ofstream fout("../image.ppm", ios::out | ios::binary);

    ASSERT(fout.is_open(), "Error opening file");

    ppm_t ppm(&fout, 250, 250);

    ppm.line(color_t(color_t::red), 10, 127, 50, 240, 150);

    ppm.show();

    fout.close();

    return 0;
}
