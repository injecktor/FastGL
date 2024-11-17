#ifndef IMAGE_GENERATOR_H
#define IMAGE_GENERATOR_H

#include <vector>

#include "color.h"

using namespace std;

class image_generator_t {
public:
    virtual ~image_generator_t() = default;
    virtual void init() = 0;
    virtual unsigned generate(const vector<color_t>& image_buffer) = 0;
    virtual string get_format_extension() = 0;
};

#endif //IMAGE_GENERATOR_H
