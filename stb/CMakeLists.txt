cmake_minimum_required(VERSION 3.18)

include_directories(${CMAKE_SOURCE_DIR}/include/stb)
add_library(stb STATIC ${CMAKE_CURRENT_SOURCE_DIR}/stb_image.cpp)