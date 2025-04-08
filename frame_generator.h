#ifndef FRAME_GENERATOR_H
#define FRAME_GENERATOR_H

#include <vector>

#include "color.h"

class frame_generator_t {
public:
    virtual ~frame_generator_t() = default;

    virtual void init() = 0;
	virtual void start_frame() = 0;
	virtual void end_frame() = 0;
	virtual void render() = 0;
	virtual void clear() = 0;
	virtual void create_window(unsigned width, unsigned height) = 0;
};

#endif //FRAME_GENERATOR_H