//
// Created by coderobe on 11.10.16.
//

#ifndef INC_0X20_COLORS_STATE_H
#define INC_0X20_COLORS_STATE_H

#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include <stdlib.h>
#include <string.h>
#include <portaudio.h>

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_PRIVATE
#define NK_IMPLEMENTATION
#define NK_GLFW_GL2_IMPLEMENTATION
#include "nuklear.h"
#include "nuklear_glfw_gl2.h"

using namespace std;

struct SongBuffer {
    string title;
    string filename;
    vector<long> buffer;
};

struct GlobalState {
    GLFWwindow* window = NULL;
    int width = 0;
    int height = 0;
    struct nk_context *nuklear_context = NULL;
    struct nk_color background_color;
    vector<SongBuffer*> songs;
    SongBuffer* current_song;
    int current_song_id;
    PaStream* audio_stream;
};

#endif //INC_0X20_COLORS_STATE_H
