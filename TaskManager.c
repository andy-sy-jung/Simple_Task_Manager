#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <dirent.h>
#include <sys/stat.h>
#include <assert.h>
#include <limits.h>
#include <time.h>
#include <errno.h>
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl.h>
#include <GLFW/glfw3.h>
#define NK_COS
#define NK_SIN
#define NK_INCLUDE_DEFAULT_ALLOCATOR
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


#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

char* readFromData(int i){
    char filename[16];
    sprintf(filename, "%d.txt", i);
    char path[PATH_MAX];
    sprintf(path, "./tasks/%s", filename);
    FILE *file = fopen(path, "r");
    if(file == NULL){
        fprintf(stderr, "Failed to read file. %s.", strerror(errno));
        return NULL;
    }
    char buf[256];
    if (fgets(buf, sizeof(buf), file) == NULL){
        fclose(file);
        return NULL;
    }
    else{
        fclose(file);
        return strdup(buf);
    }
}

void todo_status(struct nk_context *ctx, int numOfTasks, int height){
    char msg[48];
    sprintf(msg, "You have %d uncompleted tasks.", numOfTasks);
    if (nk_begin(ctx, "Status Window", nk_rect(0, 0, 195, height),
                  NK_WINDOW_NO_SCROLLBAR)) {
        nk_layout_row_dynamic(ctx, 50, 1);
        nk_label(ctx, "Hello, Andrew!", NK_TEXT_CENTERED);
        nk_layout_row_dynamic(ctx, 100, 1);
        nk_label_wrap(ctx, msg);
    }
    nk_end(ctx);
}

void main_window(struct nk_context *ctx, int width, int height, int numOfTasks){
    if (nk_begin(ctx, "Main Window", nk_rect(195, 0, width-195, height),
                 NK_WINDOW_SCROLL_AUTO_HIDE | NK_WINDOW_BORDER)) {
        nk_layout_row_dynamic(ctx, 50, 1);
        nk_label(ctx, "Current Tasks",NK_TEXT_CENTERED);
        if(numOfTasks == 0){
            nk_layout_row_dynamic(ctx, height/2, 1);
            nk_label(ctx, "There are no tasks.", NK_TEXT_CENTERED);
        }
        else{
            for(int i = 1; i < numOfTasks + 1; i++){
                char msg[256];
                nk_layout_row_dynamic(ctx, 50, 1);
                char *line = readFromData(i);
                if(line == NULL){
                    continue;
                }
                sprintf(msg, "%s", line);
                msg[sizeof(msg)-1] = '\0';
                nk_label_wrap(ctx, msg);
                free(line);
            }
        }
    }
    nk_end(ctx);
}


int main(void) {
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }
    int width = 0, height = 0;

    // Create a GLFW window
    GLFWwindow* window = glfwCreateWindow(800, 600, "Task Manager", NULL, NULL);
    if (!window) {
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        return EXIT_FAILURE;
    }

    // Make the window's OpenGL context current
    glfwMakeContextCurrent(window);
    glfwGetWindowSize(window, &width, &height);

    // Initialize Nuklear context with GLFW and OpenGL
    struct nk_context *ctx = nk_glfw3_init(window, NK_GLFW3_INSTALL_CALLBACKS);
    struct nk_colorf bg;
    bg.r = 0.10f, bg.g = 0.18f, bg.b = 0.24f, bg.a = 1.0f;

    {struct nk_font_atlas *atlas;
        nk_glfw3_font_stash_begin(&atlas);
        struct nk_font *header = nk_font_atlas_add_from_file(atlas, "./extra_font/ProggyClean.ttf", 20, 0);
        nk_glfw3_font_stash_end();
        nk_style_set_font(ctx, &header->handle);}


    // Main loop
    while (!glfwWindowShouldClose(window)) {
        // Set up Nuklear
        nk_glfw3_new_frame();

        // Rendering code
        todo_status(ctx, 2, height);
        main_window(ctx, width, height, 2);

        glfwGetWindowSize(window, &width, &height);
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(bg.r, bg.g, bg.b, bg.a);
        nk_glfw3_render(NK_ANTI_ALIASING_ON);

        // Swap buffers
        glfwSwapBuffers(window);

        // Process events
        glfwPollEvents();
    }

    // Clean up Nuklear and terminate GLFW
    nk_glfw3_shutdown();
    glfwTerminate();



    return 0;
}
