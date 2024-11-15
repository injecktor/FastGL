#ifndef PPM_H
#define PPM_H
#include <cstdint>
#include <fstream>
#include <vector>

#include "color.h"

using namespace std;

class ppm_t {
public:
    explicit ppm_t(ofstream* file, unsigned width, unsigned height);
    void line();
    void set_border(color_t color, line_t line_type);
    void show();

private:
    ofstream* m_file;
    string m_file_extension = ".ppm";
    string m_ppm_init = "P3";
    unsigned m_width;
    unsigned m_height;
    vector<array<uint8_t, 3>> m_image_buffer;
};

#endif //PPM_H
