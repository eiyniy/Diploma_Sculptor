cmake_minimum_required(VERSION 3.10)

project(SOIL_lib)

add_library(soil STATIC
  	src/image_helper.c
  	src/stb_image_aug.c
  	src/image_DXT.c
  	src/SOIL.c
)

target_include_directories(soil PUBLIC includes)

export(TARGETS soil FILE ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.cmake)
