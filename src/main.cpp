#include <iostream>
#include <random>
#include <time.h>
#include <algorithm>
#include <chrono>   // for sleep
#include <thread>

#include <glad/glad.h>  // Manages function pointers
#include <glfw3.h>      // Manages window
#include "SGLEngine.hpp"
#include "ShaderManager.hpp"
#include "Triangle.hpp"
#include "SGLRect.hpp"
#include "SortingRects.hpp"

#define SORTDELAY 1  // 0.01s (in ms)
void process_input(GLFWwindow *window);
void gl_check_error();

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
bool compareRect(SGLRect* r1, SGLRect* r2) {
    return (r1->getHeight() < r2->getHeight());
}
int main() {
    srand(time(NULL));

    std::ios_base::sync_with_stdio(0);
    std::cout << "Compile Time: " << __TIME__ << std::endl;
    SGLEngine engine = SGLEngine(SCREEN_WIDTH, SCREEN_HEIGHT);
    
    if (engine.createWindow("Sorting") == -1) return -1;    // engine handles window creation
    GLFWwindow* window = engine.get_window();

    ShaderManager shaderManager;
    shaderManager.load_shader("Triangle");
    shaderManager.load_shader("SortingRect");
    ShaderProgram* rectShader = shaderManager.getShader("SortingRect");
    SGLRect r = SGLRect(0.2, 0.7, rectShader);
    if (rectShader == NULL) {
        return 1;
    }
    SortingRects sr = SortingRects(50, rectShader);
    
    /*
     * Eventually this while loop will be replaced with a loop within sorting rectangles
     * Or perhaps, we will externally sort sr within our own loop here.
     */
    for (int i = 0; i < (int) sr.rects.size(); i++) {
        SGLRect* key = sr.rects[i];
        int j = i;
        while (j > 0 && sr.rects[j-1]->getHeight() > key->getHeight()) {
            sr.rects[j] = sr.rects[j-1];
            sr.rects[j]->setColor(0.6, 0.2, 0.2);
            sr.rects[j-1]->setColor(0.2, 0.2, 0.6);
            process_input(window);    
            glClearColor(0.1f, 0.1, 0.2f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            rectShader->use();
            sr.draw();
            glfwSwapBuffers(window);
            glfwPollEvents();
            sr.resetColor(sr.rects[j]);
            sr.resetColor(sr.rects[j-1]);
            std::this_thread::sleep_for(std::chrono::milliseconds(SORTDELAY));
            j--;
        }
        sr.rects[j] = key;
    }
    while(!glfwWindowShouldClose(window)) {
        process_input(window);    
        glClearColor(0.1f, 0.1, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        rectShader->use();
        sr.draw();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

}

// Processes user input
void process_input(GLFWwindow *window) {
    // ESC -> Close Window

    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}


void gl_check_error() {
    GLenum err = GL_NO_ERROR;
    err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cout << "Error: " << err << "\n";
    }
}