#ifndef PPM_H
#define PPM_H
#include <cstdint>
#include <fstream>
#include <vector>

#include "color.h"
#include "image_generator.h"

using namespace std;

class ppm_t final : public image_generator_t {
public:
    explicit ppm_t(ofstream *file, unsigned width, unsigned height);

    void init();

    unsigned generate(const vector<color_t> &image_buffer) override;

    string get_format_extension() override;

private:
    ofstream *m_file;
    string m_file_extension = "ppm";
    string m_ppm_init = "P3";
    unsigned m_width;
    unsigned m_height;
};

#endif //PPM_H
