#ifndef CGE_PPM_H
#define CGE_PPM_H
#include <cstdint>
#include <fstream>
#include <vector>

#include "color.h"
#include "image_format.h"

class ppm_t final : public image_format_t {
public:
    explicit ppm_t(std::ofstream *file, unsigned width, unsigned height);

    void init() override;

    unsigned generate(const std::vector<color_t> &image_buffer, const color_t background) override;

    std::string get_format_extension() override;

private:
    std::ofstream *m_file;
    std::string m_file_extension = "ppm";
    std::string m_ppm_init = "P3";
    unsigned m_width;
    unsigned m_height;
};

#endif //CGE_PPM_H
