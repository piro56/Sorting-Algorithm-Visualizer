#include <iostream>
#include <random>
#include <time.h>

#include <glad/glad.h>  // Manages function pointers
#include <glfw3.h>      // Manages window
#include "SGLEngine.hpp"
#include "ShaderManager.hpp"
#include "Triangle.hpp"
#include "SGLRect.hpp"
#include "SortingRects.hpp"

void process_input(GLFWwindow *window);
void gl_check_error();

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
int main() {
    srand(time(NULL));

    std::ios_base::sync_with_stdio(0);
    std::cout << "Compile Time: " << __TIME__ << std::endl;
    SGLEngine engine = SGLEngine(SCREEN_WIDTH, SCREEN_HEIGHT);
    
    if (engine.createWindow("Sorting") == -1) return -1;    // engine handles window creation
    GLFWwindow* window = engine.get_window();

    ShaderManager shaderManager;
    shaderManager.load_shader("Triangle");
    shaderManager.load_shader("Rectangle");
    ShaderProgram* rectShader = shaderManager.getShader("Rectangle");
    SGLRect r = SGLRect(0.2, 0.7, rectShader);
    if (rectShader == NULL) {
        return 1;
    }
    SortingRects sr = SortingRects(100, rectShader);
    
    while(!glfwWindowShouldClose(window)) {
        process_input(window);    

        glClearColor(0.2f, 0.2, 0.9f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        rectShader->use();
        //r.draw();
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