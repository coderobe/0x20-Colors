cmake_minimum_required(VERSION 3.6)
project(0x20_Colors)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
find_package(GLEW REQUIRED)
find_package(GLUT REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLFW REQUIRED)

if (NOT CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR)
    # list of files for which we add a copy rule
    set(data_SHADOW launch.sh)

    foreach(item IN LISTS data_SHADOW)
        message(STATUS ${item})
        add_custom_command(
                OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${item}"
                COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/${item}" "${CMAKE_CURRENT_BINARY_DIR}/${item}"
                DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/${item}"
        )
    endforeach()
endif()

set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} ./)
include_directories(.)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -g")
if(CMAKE_COMPILER_IS_GNUXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Og")
endif()

set(SOURCE_FILES main.cpp state.cpp nuklear.h nuklear_glfw_gl2.h state.h audio.cpp audio.h "${CMAKE_CURRENT_BINARY_DIR}/launch.sh")

add_executable(0x20_Colors ${SOURCE_FILES})

TARGET_LINK_LIBRARIES(0x20_Colors ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} ${GLUT_LIBRARIES} ${GLFW_LIBRARIES} m ${CMAKE_CURRENT_BINARY_DIR}/${BASS_LIBRARY} vorbisfile)
