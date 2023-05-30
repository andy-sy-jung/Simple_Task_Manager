#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <dirent.h>
#include <sys/stat.h>
#include <assert.h>
#include <limits.h>
#include <time.h>
#include <errno.h>
#include <unistd.h>
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl.h>
#include <GLFW/glfw3.h>
#define NK_INCLUDE_STANDARD_BOOL
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

//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"


#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

char *globalBuf;
char input[1024];
int input_length;
int numOfTasks;

int allocateMemory(void) {
    globalBuf = malloc(1024 * sizeof(char));
    if(globalBuf == NULL){
        fprintf(stderr, "Malloc failed. %s.\n", strerror(errno));
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int countTasks(void){
    int count = 0;
    DIR *dir;
    struct dirent *entry;
    dir = opendir("./tasks");
    if(dir == NULL){
        fprintf(stderr, "Failed to open directory. %s.\n", strerror(errno));
        return -1;
    }
    while((entry = readdir(dir)) != NULL){
        if(strcmp(entry->d_name, ".") == 0 ||
           strcmp(entry->d_name, "..") == 0){
            continue;
        }
        if(entry->d_type == DT_REG && entry->d_name[0] != '.'){
            count++;
        }
    }
    closedir(dir);
    return count;
}

char* readFromData(int i){
    if(allocateMemory()){
        return NULL;
    }
    char filename[16];
    sprintf(filename, "%d.txt", i);
    char path[PATH_MAX];
    sprintf(path, "./tasks/%s", filename);
    FILE *file = fopen(path, "r");
    if(file == NULL){
        fprintf(stderr, "Failed to read file. %s.", strerror(errno));
        return NULL;
    }
    if (fgets(globalBuf, 1024, file) == NULL){
        fclose(file);
        return NULL;
    }
    else{
        fclose(file);
        return globalBuf;
    }
}

void writeToData(char *buf){
    numOfTasks++;
    char filePath[PATH_MAX];
    sprintf(filePath, "./tasks/%d.txt", numOfTasks);
    FILE *file = fopen(filePath, "w");
    fprintf(file, "%s", buf);
    fclose(file);
}

void deleteData(int i){
    char fileToDelete[PATH_MAX];
    sprintf(fileToDelete, "./tasks/%d.txt", i);
    int notRemoved = remove(fileToDelete);
    if(notRemoved){
        fprintf(stderr, "Failed to delete file. %s.\n", strerror(errno));
    }
    for(int j = i; j < numOfTasks + 1; j++){
        if(j == i){
            continue;
        }
        char file1[16];
        char file2[16];
        int next = j - 1;
        char pathOld[64] = "./tasks/";
        char pathNew[64] = "./tasks/";
        sprintf(file1, "%d.txt", j);
        sprintf(file2, "%d.txt", next);
        strcat(pathOld, file1);
        strcat(pathNew, file2);
        if(rename(pathOld, pathNew)){
            fprintf(stderr, "Failed to rename file. %s.\n", strerror(errno));
        }
    }
    if(!notRemoved){
        numOfTasks--;
    }

}

void todo_status(struct nk_context *ctx, int numOfTasks, int height){
    char msg[48];
    sprintf(msg, "You have %d uncompleted tasks.", numOfTasks);
    if (nk_begin(ctx, "Status Window", nk_rect(0, 0, 205, height),
                   NK_WINDOW_NO_SCROLLBAR)) {
        nk_layout_row_dynamic(ctx, 50, 1);
        nk_label(ctx, "Hello, Andrew!", NK_TEXT_CENTERED);
        nk_layout_row_dynamic(ctx, 100, 1);
        nk_label_wrap(ctx, msg);
    }
    nk_end(ctx);
}

void entry_window(struct nk_context *ctx, int width, int height) {
    if (nk_begin(ctx, "Entry Window", nk_rect(205, 400, width-205, height-400),
                 NK_WINDOW_MINIMIZABLE | NK_WINDOW_NO_SCROLLBAR)){
        nk_layout_row_dynamic(ctx, 100, 1);
        nk_edit_string(ctx, NK_EDIT_BOX , input, &input_length, 1023, nk_filter_default);
        if(nk_input_is_key_pressed(&ctx->input, NK_KEY_ENTER)) {
            input[input_length-1] = '\0';
            if(strlen(input) != 0){
                writeToData(input);
            }
            memset(input, '\0', sizeof(input));
            input_length = 0;
        }
    }
    nk_end(ctx);
}

void main_window(struct nk_context *ctx, int width, int height, int numOfTasks){
    if (nk_begin(ctx, "Main Window", nk_rect(205, 0, width-205, 400),
                 NK_WINDOW_SCROLL_AUTO_HIDE | NK_WINDOW_BORDER)) {
        nk_layout_row_dynamic(ctx, 50, 1);
        nk_label(ctx, "Current Tasks",NK_TEXT_CENTERED);
        if(numOfTasks == 0){
            nk_layout_row_dynamic(ctx, height/2, 1);
            nk_label(ctx, "There are no tasks.", NK_TEXT_CENTERED);
        }
        else{
            for(int i = 1; i < numOfTasks + 1; i++){
                if(readFromData(i) == NULL){
                    continue;
                }
                nk_layout_row_begin(ctx, NK_STATIC, 25, 2);
                nk_layout_row_push(ctx, width-270);
                nk_label(ctx, "", NK_TEXT_ALIGN_LEFT);
                nk_layout_row_push(ctx, 25);
                if(nk_button_symbol(ctx, NK_SYMBOL_X)){
                    printf("Button %d Clicked.\n", i);
                    deleteData(i);
                }
                nk_layout_row_end(ctx);
                nk_layout_row_dynamic(ctx, 75, 1);
                nk_labelf_wrap(ctx,"%s", readFromData(i));
                free(globalBuf);
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

    numOfTasks = countTasks();

    {struct nk_font_atlas *atlas;
        nk_glfw3_font_stash_begin(&atlas);
        struct nk_font *header = nk_font_atlas_add_from_file(atlas, "./extra_font/ProggyClean.ttf", 20, 0);
        nk_glfw3_font_stash_end();
        nk_style_set_font(ctx, &header->handle);
    }

    // Main loop
    while (!glfwWindowShouldClose(window)) {

        // Set up Nuklear
        nk_glfw3_new_frame();

        // Rendering code
        todo_status(ctx, numOfTasks, height);
        main_window(ctx, width, height, numOfTasks);
        entry_window(ctx, width, height);

        glfwGetWindowSize(window, &width, &height);
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(bg.r, bg.g, bg.b, bg.a);
        nk_glfw3_render(NK_ANTI_ALIASING_ON);

        // Swap buffers
        glfwSwapBuffers(window);

        // Process events
        glfwPollEvents();
        glfwWaitEvents();
    }

    // Clean up Nuklear and terminate GLFW
    nk_glfw3_shutdown();
    glfwTerminate();



    return 0;
}
