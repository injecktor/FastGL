cmake_minimum_required(VERSION 3.22)
project(fastgl)

set(CMAKE_CXX_STANDARD 20)

add_executable(fastgl main.cpp
        logging.h
        color.cpp
        color.h
        line.cpp
        line.h
        draw_process.cpp
        draw_process.h
        image_format.h

        image_formats/gl_png.cpp
        image_formats/gl_png.h
        image_formats/gl_ppm.cpp
        image_formats/gl_ppm.h
        )

target_include_directories(fastgl PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}
        image_formats
        )