#include <iostream>
#include <random>
#include <time.h>
#include <algorithm>
#include <chrono>   // for sleep
#include <thread>
#include <atomic>
#include <mutex>

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

#include <glad/glad.h>  // Manages function pointers
#include <glfw3.h>      // Manages window
#include "SGLEngine.hpp"
#include "ShaderManager.hpp"
#include "Triangle.hpp"
#include "SGLRect.hpp"
#include "SortingRects.hpp"
#include "Sorting.hpp"

#define SORT_DELAY_DEFAULT 10000  // in microseconds


int process_input(GLFWwindow *window);
void gl_check_error();

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const char* glsl_version = "#version 150";
bool compareRect(SGLRect* r1, SGLRect* r2) {
    return (r1->getHeight() < r2->getHeight());
}
void insertion_sort(SortingRects* sr, GLFWwindow* window, ShaderProgram* rectShader);
void threaded_insertion_sort(SortingRects*, std::atomic<bool>*);
void threaded_merge_sort(SortingRects* const sr, std::atomic<bool>* const, int l, int r);
void merge(SortingRects* const sr, std::atomic<bool>* const, int l, int m, int r);
std::atomic<int> SORTING_DELAY(SORT_DELAY_DEFAULT);
std::mutex rectLock;

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
    SortingRects sr = SortingRects(100, rectShader);
    Sorting s = Sorting(10, rectShader);

    std::atomic<bool> programRunning(true);
    //std::thread sort_task(threaded_insertion_sort, &sr, &programRunning);
    std::thread sort_task(threaded_merge_sort, &sr, &programRunning, 0, sr.rects.size() - 1);

    // GUI
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io; 
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    ImGui::SetNextWindowSize(ImVec2(SCREEN_HEIGHT/4, SCREEN_WIDTH/4));
    float SORT_MULTIPLIER = 1.0f;
    bool FINISH = false;
    while(!glfwWindowShouldClose(window)) {
        int input = process_input(window);
        (void) input;
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
            ImGui::Checkbox("Speedup", &FINISH);            
            ImGui::SliderFloat("Speed", &SORT_MULTIPLIER, 0.1, 10.0, "%.3f", ImGuiSliderFlags_Logarithmic);
            SORTING_DELAY.store((SORT_DELAY_DEFAULT/ SORT_MULTIPLIER), std::memory_order_relaxed);
            if (FINISH) {
                SORTING_DELAY.store(0, std::memory_order_relaxed);
            }
            ImGui::End();
        }
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    programRunning.store(false, std::memory_order_relaxed);
    sort_task.join();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

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
            std::this_thread::sleep_for(std::chrono::microseconds(
                                SORTING_DELAY.load(std::memory_order_relaxed)));
            j--;
        }
        sr->rects[j] = key;
    }
 }

void threaded_insertion_sort(SortingRects* sr, std::atomic<bool>* run) {
    for (int i = 0; i < (int) sr->rects.size(); i++) {
        SGLRect* key = sr->rects[i];
        int j = i;
        if (!run->load(std::memory_order_relaxed)) return;
        while (j > 0 && sr->rects[j-1]->getHeight() > key->getHeight()) {
            rectLock.lock();
            sr->rects[j] = sr->rects[j-1];
            sr->rects[j]->setColor(0.6, 0.2, 0.2);
            sr->rects[j-1]->setColor(0.2, 0.2, 0.6);
            rectLock.unlock();

            std::this_thread::sleep_for(std::chrono::microseconds(
                    SORTING_DELAY.load(std::memory_order_relaxed)));

            rectLock.lock();
            sr->resetColor(sr->rects[j-1]);
            sr->resetColor(sr->rects[j]);
            rectLock.unlock();
            j--;
        }
        rectLock.lock();
        sr->rects[j] = key;
        rectLock.unlock();
    }
}

void threaded_merge_sort(SortingRects* const sr, std::atomic<bool>* const run, int l, int r) {
    if (!run->load(std::memory_order_relaxed)) return;
    if (l >= r) return;

    // find middle
    int middle = l + (r - l) / 2; // floor(l+r/2)

    threaded_merge_sort(sr, run, l, middle);
    if (!run->load(std::memory_order_relaxed)) return;
    threaded_merge_sort(sr, run, middle+1, r);
    
    // merge two halves;
    merge(sr, run, l, middle, r);
    if (!run->load(std::memory_order_relaxed)) return;\
}

void merge(SortingRects* const sr, std::atomic<bool>* const run, int l, int m, int r) {
    if (!run->load(std::memory_order_relaxed)) return;
    // sizes for new arrays
    int arrOne = m - l + 1;
    int arrTwo = r - m; 

    // copy in sections of sr->rects into temp vectors
    
    rectLock.lock();
    std::vector<SGLRect*> leftArr(sr->rects.begin() + l, sr->rects.begin() + l + arrOne);
    std::vector<SGLRect*> rightArr(sr->rects.begin() + m + 1, sr->rects.begin() + m + 1 + arrTwo);

    // set their colors
    for (auto i = sr->rects.begin() + l; i != sr->rects.begin() + m + 1 + arrTwo; i++) {
        (*i)->setColor(0.3, 0.3, 1.0);
    }
    rectLock.unlock();
    
    int leftIndex = 0, rightIndex = 0;
    int mergedIndex = 0;

    // merge arrays
    
    while(leftIndex < arrOne && rightIndex < arrTwo) {
        // bad if we exit program here since we need to cleanup array, so we can skip delay if we do need to close
        if (run->load(std::memory_order_relaxed)) {
        std::this_thread::sleep_for(std::chrono::microseconds(
                    SORTING_DELAY.load(std::memory_order_relaxed)));
        }
        if (leftArr[leftIndex]->getHeight() <= rightArr[rightIndex]->getHeight()) {
            rectLock.lock();
            sr->rects[l + mergedIndex] = leftArr[leftIndex];
            rectLock.unlock();
            leftIndex++;
            mergedIndex++;
        } else {
            rectLock.lock();
            sr->rects[l + mergedIndex] = rightArr[rightIndex];
            rectLock.unlock();
            rightIndex++;
            mergedIndex++;
        }
    }
    // copy in remaining

    while (rightIndex < arrTwo) {
        sr->rects[l+mergedIndex] = rightArr[rightIndex];
        rightIndex++;
        mergedIndex++;
    }
    while (leftIndex < arrOne) {
        sr->rects[l+mergedIndex] = leftArr[leftIndex];
        leftIndex++;
        mergedIndex++;
    }
    if (!run->load(std::memory_order_relaxed)) return;
    rectLock.lock();
    for (auto i = sr->rects.begin() + l; i != sr->rects.begin() + m + 1 + arrTwo; i++) {
        sr->resetColor(*i);
    }
    rectLock.unlock();
}