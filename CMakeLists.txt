cmake_minimum_required(VERSION 3.22)
project(cge)

set(CMAKE_CXX_STANDARD 20)

add_executable(cge main.cpp
        logging.h
        color.cpp
        color.h
        line.cpp
        line.h
        frame_process.cpp
        frame_process.h
        frame_generator.h
        image_format.h
        include_graphic_core.h
        
        frame_generators/cge_sdl2.cpp
        frame_generators/cge_sdl2.h

        image_formats/cge_png.cpp
        image_formats/cge_png.h
        image_formats/cge_ppm.cpp
        image_formats/cge_ppm.h
        )

target_include_directories(cge PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}
        image_formats
        frame_generators)

target_compile_definitions(cge PRIVATE
        CGE_CORE_SDL2)