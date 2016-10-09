#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <math.h>
#include <limits.h>
#include <time.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_GLFW_GL2_IMPLEMENTATION

#include "nuklear.h"
#include "nuklear_glfw_gl2.h"

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 800

#define MAX_VERTEX_BUFFER 512 * 1024
#define MAX_ELEMENT_BUFFER 128 * 1024

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"
#include "util.h"

int main(void) {
    /* Platform */
    static GLFWwindow *win;
    int width = 0, height = 0;
    struct nk_context *ctx;
    struct nk_color background;

    /* GLFW */
    glfwSetErrorCallback(error_callback);
    if (!glfwInit()) {
        fprintf(stdout, "[GFLW] failed to init!\n");
        exit(1);
    }
    win = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "0xNative", NULL, NULL);
    glfwMakeContextCurrent(win);
    glfwGetWindowSize(win, &width, &height);

    /* GUI */
    ctx = nk_glfw3_init(win, NK_GLFW3_INSTALL_CALLBACKS);
    {
        struct nk_font_atlas *atlas;
        nk_glfw3_font_stash_begin(&atlas);
        nk_glfw3_font_stash_end();
    }

    background = nk_rgb(0, 0, 0);

    struct nk_image background_image = icon_load("./Megumi.png");

    while (!glfwWindowShouldClose(win)) {
        /* Input */
        glfwPollEvents();
        nk_glfw3_new_frame();

        /* GUI */
        /*
        {
            struct nk_panel layout;
            if (nk_begin(ctx, &layout, "0xNative image test", nk_rect(320, 50, 275, 610),
                         NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_TITLE | NK_WINDOW_SCALABLE)
                    ) {
                static const float ratio[] = {1.0f, 1.0f, 1.0f};
                nk_layout_row(ctx, NK_DYNAMIC, height, 3, ratio);
                nk_spacing(ctx, 1);
                nk_image(ctx, background_image);
                //printf("test");
            }
            nk_end(ctx);
        }
        */

        /* Draw */
        {
            float bg[4];
            nk_color_fv(bg, background);
            glfwGetWindowSize(win, &width, &height);
            glViewport(0, 0, width, height);
            glClear(GL_COLOR_BUFFER_BIT);
            glClearColor(bg[0], bg[1], bg[2], bg[3]);
            /* IMPORTANT: `nk_glfw_render` modifies some global OpenGL state
             * with blending, scissor, face culling and depth test and defaults everything
             * back into a default state. Make sure to either save and restore or
             * reset your own state after drawing rendering the UI. */
            nk_glfw3_render(NK_ANTI_ALIASING_ON, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);
            glfwSwapBuffers(win);
        }
    }

    nk_glfw3_shutdown();
    glfwTerminate();

    return 0;
}