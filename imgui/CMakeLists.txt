cmake_minimum_required(VERSION 3.21)
include_directories(${CMAKE_SOURCE_DIR}/include/imgui)
add_library(imgui STATIC
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui_draw.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui_widgets.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui_tables.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui_impl_glfw.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui_impl_opengl3.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui_stdlib.cpp
)