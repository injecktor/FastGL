#ifndef IMAGE_FORMAT_H
#define IMAGE_FORMAT_H

#include <vector>

#include "color.hpp"

class image_format_t {
public:
    virtual ~image_format_t() = default;

    virtual void init() = 0;
    virtual unsigned generate(const std::vector<color_t> &image_buffer, const color_t background) = 0;
    virtual std::string get_format_extension() = 0;
};

#endif //IMAGE_FORMAT_H
