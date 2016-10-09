cmake_minimum_required(VERSION 3.6)
project(0xNative)

if (NOT CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR)
    # list of files for which we add a copy rule
    set(data_SHADOW Megumi.png)

    foreach(item IN LISTS data_SHADOW)
        message(STATUS ${item})
        add_custom_command(
                OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${item}"
                COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/${item}" "${CMAKE_CURRENT_BINARY_DIR}/${item}"
                DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/${item}"
        )
    endforeach()
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++14 -Og -g")

set(SOURCE_FILES
    main.cpp
    nuklear.h
    nuklear_glfw_gl2.h
    stb_image.h

        "${CMAKE_CURRENT_BINARY_DIR}/Megumi.png")

add_executable(0xNative ${SOURCE_FILES})

TARGET_LINK_LIBRARIES(0xNative GLEW glfw GL m GLU)
