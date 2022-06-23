#pragma once
#include <iostream>
#include <time.h>
#include <algorithm>
#include <chrono> // for sleep
#include <thread>
#include <atomic>
#include <mutex>

#include <glad/glad.h>
#include <glfw3.h>

#include "SGLRect.hpp"
#include "SortingRects.hpp"

#define DEBUG_SALG
#define HEAP_LEFT(i) (2 * (i) + 1)
#define HEAP_RIGHT(i) (2 * (i) + 2)
#define PAUSE_DELAY 100

/* Requires: std::atomic<bool> stop_flag, pause ----------------------------*/
/* If stop, return out. If pause, sleep.                                    */
#define PAUSESTOP(RETURN_VAL)                                                \
    if (stop_flag->load(std::memory_order_relaxed))                          \
        return RETURN_VAL;                                                   \
    while (pause->load(std::memory_order_relaxed))                           \
    {                                                                        \
        if (stop_flag->load(std::memory_order_relaxed))                      \
            return RETURN_VAL;                                               \
        std::this_thread::sleep_for(std::chrono::milliseconds(PAUSE_DELAY)); \
    }                                                                        \
    /*--------------------------------------------------------------------------*/

enum SORT_ALG
{
    INSERTION = 0,
    MERGESORT = 1,
    HEAPSORT = 2,
    QUICKSORT = 3
};
void insertion_sort(SortingRects *sr, GLFWwindow *window, ShaderProgram *rectShader);
void threaded_insertion_sort(SortingRects *sr, std::atomic<int> *delay,
                             std::atomic<bool> *stop_flag, std::atomic<bool> *pause, std::mutex *rectLock);
void threaded_merge_sort(SortingRects *const sr, std::atomic<int> *delay, std::atomic<bool> *stop_flag,
                         std::atomic<bool> *pause, std::mutex *rectLock, int l, int r);
void merge(SortingRects *const sr, std::atomic<int> *delay, std::atomic<bool> *stop_flag,
           std::atomic<bool> *pause, std::mutex *rectLock, int l, int m, int r);

void heap_sort(SortingRects *const sr, std::atomic<int> *delay, std::atomic<bool> *stop_flag,
               std::atomic<bool> *pause, std::mutex *rectLock);
void maxHeapify(SortingRects *const sr, std::atomic<int> *delay, std::atomic<bool> *stop_flag,
                std::atomic<bool> *pause, std::mutex *rectLock, const size_t rootIndex, const size_t heapSize);
void buildMaxHeap(SortingRects *const sr, std::atomic<int> *delay, std::atomic<bool> *stop_flag,
                  std::atomic<bool> *pause, std::mutex *rectLock);

void quickSort(SortingRects *const sr, std::atomic<int> *delay, std::atomic<bool> *stop_flag,
               std::atomic<bool> *pause, std::mutex *rectLock, int p, int r);
int partition(SortingRects *const sr, std::atomic<int> *delay, std::atomic<bool> *stop_flag,
              std::atomic<bool> *pause, std::mutex *rectLock, int p, int r);

/* Runs SortingAlgorithm on SortingRects and manages threads.
 * Can pause or unpause threads
 * Can change what algorithm is being run on the thread
 */
class SortingAlgs
{
private:
    static const int SORT_DELAY_DEFAULT = 10000;
    std::thread *sort_task;
    std::mutex *vectorAccessLock;
    std::atomic<int> *SORTING_DELAY;
    std::atomic<bool> *paused; // if currently sorting (unpaused)
    std::atomic<bool> *stop_flag;
    bool thread_running = false;

    SortingRects *sr;

public:
    SORT_ALG currentAlg = INSERTION;

    SortingAlgs(SortingRects *sr, std::atomic<int> *SORTING_DELAY, std::mutex *access_lock);
    ~SortingAlgs();

    void pause();
    void unpause();
    void start();
    void stop();
    void start(SORT_ALG ALG);

    void reset();
    void selectAlgorithm();
};