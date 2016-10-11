#include <stdio.h>
#include <string>
#include <string.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "state.h"
#include "audio.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define MAX_VERTEX_BUFFER 512 * 1024
#define MAX_ELEMENT_BUFFER 128 * 1024

using namespace std;

GLFWerrorfun error_callback(int err, const char* errmsg){
    // Print GLFW error, err == error id, errmsg == UTF-8 encoded error message
    fprintf(stderr, "[GLFW] Error %i: %s\n", err, errmsg);
}

int main() {
    // Create new GlobalState which will contain most info about the program
    GlobalState* state = new GlobalState();

    // Initialize GLFW
    glfwSetErrorCallback((void (*)(int, const char*))error_callback);
    if(!glfwInit()){
        fprintf(stderr, "[GLFW] GLFW failed to init!\n");
        #pragma clang diagnostic push
        #pragma ide diagnostic ignored "CannotResolve"
        exit(1);
        #pragma clang diagnostic pop
    }

    state->window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "0x20 Colors", NULL, NULL);
    glfwMakeContextCurrent(state->window);
    glfwGetWindowSize(state->window, &state->width, &state->height);

    Audio* audio_engine = new Audio(state);
    audio_engine->init();
    audio_engine->play_song(0);

    // Initialize Nuklear GUI
    state->nuklear_context = nk_glfw3_init(state->window, NK_GLFW3_INSTALL_CALLBACKS);
    {
        struct nk_font_atlas* nuklear_font_atlas;
        nk_glfw3_font_stash_begin(&nuklear_font_atlas);
        nk_glfw3_font_stash_end();
    }

    // Set default background color
    state->background_color = nk_rgb(0, 100, 100);

    // Main render loop
    while(!glfwWindowShouldClose(state->window)){
        // Poll input
        glfwPollEvents();
        //Start new frame
        nk_glfw3_new_frame();

        // todo: GUI elements go here

        // Draw
        {
            float bg[4];
            nk_color_fv(bg, state->background_color);
            glfwGetWindowSize(state->window, &state->width, &state->height);
            glViewport(0, 0, state->width, state->height);
            glClear(GL_COLOR_BUFFER_BIT);
            glClearColor(bg[0], bg[1], bg[2], bg[3]);
            /* IMPORTANT: `nk_glfw_render` modifies some global OpenGL state
             * with blending, scissor, face culling and depth test and defaults everything
             * back into a default state. Make sure to either save and restore or
             * reset your own state after drawing rendering the UI. */
            nk_glfw3_render(NK_ANTI_ALIASING_ON, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);
            // Swap video buffers
            glfwSwapBuffers(state->window);
        }
    }

    // Shutdown cleanly
    nk_glfw3_shutdown();
    glfwTerminate();

    // Free memory
    delete state;

    return 0;
}