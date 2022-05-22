cmake_minimum_required(VERSION 3.21)
project(Cube)

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin )
set( CMAKE_CXX_STANDARD 23 )
add_compile_definitions(GLEW_STATIC)

include_directories(${CMAKE_SOURCE_DIR}/include)
add_subdirectory(imgui)
add_subdirectory(App)
add_subdirectory(stb)
add_executable(Cube main.cpp)
target_link_libraries(
        Cube
        App graphics stb imgui ${CMAKE_SOURCE_DIR}/libs/libglfw3.a ${CMAKE_SOURCE_DIR}/libs/glew32s.lib opengl32
)