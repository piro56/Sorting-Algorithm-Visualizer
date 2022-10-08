#include <iostream>
#include <random>
#include <time.h>
#include <algorithm>
#include <chrono> // for sleep
#include <thread>
#include <atomic>
#include <mutex>

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

#include <glad/glad.h> // Manages function pointers
#include <glfw3.h>     // Manages window
#include "SGLEngine.hpp"
#include "ShaderManager.hpp"
#include "Triangle.hpp"
#include "SGLRect.hpp"
#include "SortingRects.hpp"
#include "SortAlgs.hpp"
#define SORT_DELAY_DEFAULT 10000 // in microseconds

int process_input(GLFWwindow *window);
void gl_check_error();

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const char *glsl_version = "#version 150";
bool compareRect(SGLRect *r1, SGLRect *r2)
{
    return (r1->getHeight() < r2->getHeight());
}
void insertion_sort(SortingRects *sr, GLFWwindow *window, ShaderProgram *rectShader);
void threaded_insertion_sort(SortingRects *, std::atomic<bool> *);
void threaded_merge_sort(SortingRects *const sr, std::atomic<bool> *const, int l, int r);
void merge(SortingRects *const sr, std::atomic<bool> *const, int l, int m, int r);
std::atomic<int> SORTING_DELAY(SORT_DELAY_DEFAULT);
std::mutex rectLock;

int main()
{
    srand(time(NULL));
    std::ios_base::sync_with_stdio(0);
    std::cout << "Compile Time: " << __TIME__ << std::endl;
    SGLEngine engine = SGLEngine(SCREEN_WIDTH, SCREEN_HEIGHT);
    if (engine.createWindow("Sorting") == -1)
        return -1; // engine handles window creation
    GLFWwindow *window = engine.get_window();

    ShaderManager shaderManager;
    shaderManager.load_shader("SortingRect");
    shaderManager.load_shader("Sorting");
    ShaderProgram *rectShader = shaderManager.getShader("SortingRect");
    ShaderProgram *sortShader = shaderManager.getShader("Sorting");

    if (rectShader == NULL || sortShader == NULL)
    {
        return 1;
    }
<<<<<<< HEAD
    SortingRects sr = SortingRects(50000, rectShader);
    Sorting s = Sorting(10, rectShader);
=======
    SortingRects sr = SortingRects(100, rectShader);
>>>>>>> 0e31e42f3921f04d064e576b2e6d7b6fdf9a3021

    std::atomic<bool> programRunning(true);
    // std::thread sort_task(threaded_insertion_sort, &sr, &programRunning);
    // std::thread sort_task(threaded_merge_sort, &sr, &programRunning, 0, sr.rects.size() - 1);

    // GUI
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    ImGui::SetNextWindowSize(ImVec2(SCREEN_HEIGHT / 4, SCREEN_WIDTH / 4));
    float SORT_MULTIPLIER = 1.0f;
    bool FINISH = false;
    bool PAUSED = false;

    const char *items[] = {
        "Insertion Sort",
        "Merge Sort",
        "Heap Sort",
        "Quick Sort"};
    int currentAlg = 0;
    bool reset = false;
    bool start_new = false;
    SortingAlgs sorter = SortingAlgs(&sr, &SORTING_DELAY, &rectLock);

    // DRAW
    while (!glfwWindowShouldClose(window))
    {
        int input = process_input(window);
        if (start_new)
        {
            sorter.stop();
            sorter.start((SORT_ALG)currentAlg);
        }
        if (input == 4 || reset)
        {
            sorter.reset();
            sorter.start((SORT_ALG)currentAlg);
        }
        glClearColor(0.1f, 0.1, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        rectShader->use();
        rectLock.lock();
        sr.draw();
        rectLock.unlock();
        // GUI
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        {
            ImGui::Begin("Sort Settings");
            start_new = ImGui::Button("Start");
            reset = ImGui::Button("Reset");
            ImGui::SameLine();
            ImGui::Checkbox("Speedup", &FINISH);
            ImGui::SameLine();
            ImGui::Checkbox("Pause", &PAUSED);
            ImGui::SliderFloat("Speed", &SORT_MULTIPLIER, 0.1, 10.0, "%.3f", ImGuiSliderFlags_Logarithmic);
            ImGui::ListBox("listbox", &currentAlg, items, IM_ARRAYSIZE(items), 5);
            SORTING_DELAY.store((SORT_DELAY_DEFAULT / SORT_MULTIPLIER), std::memory_order_relaxed);
            if (FINISH)
            {
                SORTING_DELAY.store(0, std::memory_order_relaxed);
            }
            if (PAUSED)
            {
                sorter.pause();
            }
            else
            {
                sorter.unpause();
            }
            ImGui::End();
        }
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    sorter.stop();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

// Processes user input
int process_input(GLFWwindow *window)
{
    // ESC -> Close Window

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
        return -1;
    }
    else if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        return 4;
    }
    return 0;
}

void gl_check_error()
{
    GLenum err = GL_NO_ERROR;
    err = glGetError();
    if (err != GL_NO_ERROR)
    {
        std::cout << "Error: " << err << "\n";
    }
}