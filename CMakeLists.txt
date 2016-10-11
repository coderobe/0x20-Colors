cmake_minimum_required(VERSION 3.6)
project(0x20_Colors)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Og -g")

set(SOURCE_FILES main.cpp state.cpp nuklear.h nuklear_glfw_gl2.h state.h audio.cpp audio.h)
add_executable(0x20_Colors ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(0x20_Colors GLEW glfw GL m GLU vorbisfile)