#pragma once
#include <iostream>
#include <time.h>
#include <algorithm>
#include <chrono>   // for sleep
#include <thread>
#include <atomic>
#include <mutex>

#include <glad/glad.h>
#include <glfw3.h>

#include "SGLRect.hpp"
#include "SortingRects.hpp"

#define DEBUG_SALG

enum SORT_ALG {
    INSERTION, MERGESORT
};
void insertion_sort(SortingRects* sr, GLFWwindow* window, ShaderProgram* rectShader);
void threaded_insertion_sort(SortingRects* sr, std::atomic<int>* delay, 
                            std::atomic<bool>* stop_flag, std::atomic<bool>* pause, std::mutex* rectLock);
void threaded_merge_sort(SortingRects* const sr, std::atomic<int>* delay, std::atomic<bool>* stop_flag, 
                        std::atomic<bool>* pause, std::mutex* rectLock, int l, int r);
void merge(SortingRects* const sr, std::atomic<int>* delay, std::atomic<bool>* stop_flag, 
                        std::atomic<bool>* pause, std::mutex* rectLock, int l, int m, int r);


/* Runs SortingAlgorithm on SortingRects and manages threads.
 * Can pause or unpause threads
 * Can change what algorithm is being run on the thread
 */ 
class SortingAlgs {
private:
static const int SORT_DELAY_DEFAULT = 10000;
std::thread* sort_task;
std::mutex* vectorAccessLock;
std::atomic<int>* SORTING_DELAY;
std::atomic<bool>* paused;     // if currently sorting (unpaused)
std::atomic<bool>* stop_flag;
bool thread_running = false;

SortingRects* sr;


public:
SORT_ALG currentAlg = INSERTION;

SortingAlgs(SortingRects* sr, std::atomic<int>* SORTING_DELAY, std::mutex* access_lock);
~SortingAlgs();

void pause();
void unpause();
void start();
void stop();
void start(SORT_ALG ALG);

void reset();
void selectAlgorithm();



};