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

    fout.close();

    return 0;
}
