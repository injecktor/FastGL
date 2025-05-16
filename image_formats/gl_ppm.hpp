#ifndef GL_PPM_H
#define GL_PPM_H

#include <cstdint>
#include <fstream>
#include <vector>

#include "color.hpp"
#include "image_format.hpp"

class ppm_t : public image_format_t {
public:
    ppm_t(std::ofstream *file, unsigned width, unsigned height);
    void generate(const std::vector<color_t> &image_buffer, color_t background) override;
    std::string get_format_extension() override;

private:
    std::ofstream *m_file;
    std::string m_file_extension = "ppm";
    std::string m_identifier = "P3";
    unsigned m_width;
    unsigned m_height;
};

#endif //GL_PPM_H
