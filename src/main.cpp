#include <iostream>
#include <random>
#include <time.h>
#include <windows.h>

#include <glad/glad.h>  // Manages function pointers
#include <glfw3.h>      // Manages window
#include "SGLEngine.h"
#include "Triangle.h"

void process_input(GLFWwindow *window);
void gl_check_error();

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
int main() {
    srand(time(NULL));

    std::ios_base::sync_with_stdio(0);
    std::cout << "Compile Time: " << __TIME__ << "\n";
    SGLEngine engine = SGLEngine(SCREEN_WIDTH, SCREEN_HEIGHT);
    
    if (engine.createWindow("Sorting") == -1) return -1;    // engine handles window creation
    GLFWwindow* window = engine.get_window();

    char result[ MAX_PATH ];
    std::string s = std::string( result, GetModuleFileName( NULL, result, MAX_PATH ) );
    std::cout << "Path" << s << std::endl;

    while(!glfwWindowShouldClose(window)) {
        process_input(window);
        glClearColor(0.2f, cos(glfwGetTime()), 0.9f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);



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