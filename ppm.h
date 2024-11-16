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
    void set_pixel(color_t color, unsigned x, unsigned y);
    void set_background(color_t color);
    void line(color_t color, unsigned width, unsigned x1, unsigned y1, unsigned x2, unsigned y2);
    void set_border(color_t color, line_t line_type);
    void show() const;

private:
    ofstream* m_file;
    string m_file_extension = ".ppm";
    string m_ppm_init = "P3";
    unsigned m_width;
    unsigned m_height;
    unsigned m_resolution;
    // if 1 it's background otherwise it's not
    vector<color_t> m_image_buffer;
    vector<uint8_t> m_background_bit_mask;
};

#endif //PPM_H
