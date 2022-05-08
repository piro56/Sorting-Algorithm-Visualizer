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
#include "Sorting.hpp"

#define SORTDELAY 50  // in ms
int process_input(GLFWwindow *window);
void gl_check_error();

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
bool compareRect(SGLRect* r1, SGLRect* r2) {
    return (r1->getHeight() < r2->getHeight());
}
void insertion_sort(SortingRects* sr, GLFWwindow* window, ShaderProgram* rectShader);
void threaded_insertion_sort(SortingRects*);

int main() {
    srand(time(NULL));
    std::ios_base::sync_with_stdio(0);
    std::cout << "Compile Time: " << __TIME__ << std::endl;
    SGLEngine engine = SGLEngine(SCREEN_WIDTH, SCREEN_HEIGHT);
    if (engine.createWindow("Sorting") == -1) return -1;    // engine handles window creation
    GLFWwindow* window = engine.get_window();




    ShaderManager shaderManager;
    shaderManager.load_shader("SortingRect");
    shaderManager.load_shader("Sorting");
    ShaderProgram* rectShader = shaderManager.getShader("SortingRect");
    ShaderProgram* sortShader = shaderManager.getShader("Sorting");

    if (rectShader == NULL || sortShader == NULL) {
        return 1;
    }

    SortingRects sr = SortingRects(50, rectShader);
    Sorting s = Sorting(10, rectShader);
    std::thread sort_task(threaded_insertion_sort, &sr);
    while(!glfwWindowShouldClose(window)) {
        int input = process_input(window);
        (void) input;
        glClearColor(0.1f, 0.1, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        rectShader->use();
        sr.draw();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    sort_task.join();


}

// Processes user input
int process_input(GLFWwindow *window) {
    // ESC -> Close Window

    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
        return -1;
    }
    else if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        return 4;
    }
    return 0;
}


void gl_check_error() {
    GLenum err = GL_NO_ERROR;
    err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cout << "Error: " << err << "\n";
    }
}
 
 void insertion_sort(SortingRects* sr, GLFWwindow* window, ShaderProgram* rectShader) {
         for (int i = 0; i < (int) sr->rects.size(); i++) {
        SGLRect* key = sr->rects[i];
        int j = i;
        while (j > 0 && sr->rects[j-1]->getHeight() > key->getHeight()) {
            sr->rects[j] = sr->rects[j-1];
            sr->rects[j]->setColor(0.6, 0.2, 0.2);
            sr->rects[j-1]->setColor(0.2, 0.2, 0.6); 
            process_input(window);
            if (glfwWindowShouldClose(window)) break;
            glClearColor(0.1f, 0.1, 0.2f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            rectShader->use();
            sr->draw();
            glfwSwapBuffers(window);
            glfwPollEvents();
            sr->resetColor(sr->rects[j]);
            sr->resetColor(sr->rects[j-1]);
            std::this_thread::sleep_for(std::chrono::milliseconds(SORTDELAY));
            j--;
        }
        sr->rects[j] = key;
    }
 }

// Handle threaded changing of values here...
// Draw separately.
// Perhaps sync?
void threaded_insertion_sort(SortingRects* sr) {
    for (int i = 0; i < (int) sr->rects.size(); i++) {
        std::this_thread::sleep_for(std::chrono::milliseconds(SORTDELAY));
        SGLRect* key = sr->rects[i];
        int j = i;
        while (j > 0 && sr->rects[j-1]->getHeight() > key->getHeight()) {
            sr->rects[j] = sr->rects[j-1];  // SYNC?
            j--;
        }
        sr->rects[j] = key; // SYNC?
    }
}