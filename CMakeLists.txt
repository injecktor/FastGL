cmake_minimum_required(VERSION 3.22)
project(fastgl)

add_subdirectory(math_tools)

set(CMAKE_CXX_STANDARD 20)

add_executable(fastgl main.cpp
        logging.hpp
        color.cpp
        color.hpp
        line.cpp
        line.hpp
        draw_process.cpp
        draw_process.hpp
        fastgl.cpp
        fastgl.hpp
        
        image_formats/image_format.hpp
        image_formats/gl_png.cpp
        image_formats/gl_png.hpp
        image_formats/gl_ppm.cpp
        image_formats/gl_ppm.hpp
        )

target_include_directories(fastgl PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}
        image_formats
        )

target_link_libraries(fastgl PRIVATE
        math_tools
        X11
        )