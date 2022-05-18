#include "SortAlgs.hpp"


void threaded_insertion_sort(SortingRects* sr, std::atomic<int>* delay, std::atomic<bool>* stop_flag, 
                            std::atomic<bool>* pause, std::mutex* rectLock) {
    for (int i = 0; i < (int) sr->rects.size(); i++) {
        SGLRect* key = sr->rects[i];
        int j = i;
        
        if (stop_flag->load(std::memory_order_relaxed)) { 
            return;
        }        
        // PAUSE LOOP
        while (pause->load(std::memory_order_relaxed)) {
            if (stop_flag->load(std::memory_order_relaxed)) return;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        while (j > 0 && sr->rects[j-1]->getHeight() > key->getHeight()) {

            rectLock->lock();
            sr->rects[j] = sr->rects[j-1];
            sr->rects[j]->setColor(0.6, 0.2, 0.2);
            sr->rects[j-1]->setColor(0.2, 0.2, 0.6);
            rectLock->unlock();
            std::this_thread::sleep_for(std::chrono::microseconds(
                    delay->load(std::memory_order_relaxed)));

            rectLock->lock();
            sr->resetColor(sr->rects[j-1]);
            sr->resetColor(sr->rects[j]);
            rectLock->unlock();
            j--;
        }
        rectLock->lock();
        sr->rects[j] = key;
        rectLock->unlock();
        if (stop_flag->load(std::memory_order_relaxed)) { 
            return;
        }
        //PAUSE LOOP
        while (pause->load(std::memory_order_relaxed)) {
            if (stop_flag->load(std::memory_order_relaxed)) return;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
}




SortingAlgs::SortingAlgs(SortingRects* sr, std::atomic<int>* delay, std::mutex* access_lock) {
this->sr = sr;
this->vectorAccessLock = access_lock;
this->SORTING_DELAY = delay;
this->paused = new std::atomic<bool>(false);
this->stop_flag = new std::atomic<bool>(false);
this->sort_task = NULL;
}

SortingAlgs::~SortingAlgs() {
    if (sort_task != NULL) {
        stop();
    }
    delete(paused);
    delete(stop_flag);
}

void SortingAlgs::pause() {
    paused->store(true, std::memory_order_relaxed);
}
void SortingAlgs::unpause() {
    paused->store(false, std::memory_order_relaxed);
}

void SortingAlgs::reset() {
    stop();
    sr->randomize();
}

void SortingAlgs::stop() {
    if (sort_task != NULL) {
    this->paused->store(false, std::memory_order_relaxed);
    this->stop_flag->store(true, std::memory_order_relaxed);
    
    // sleep for thread to join
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    if (sort_task->joinable()) {
        sort_task->join();
    }
    thread_running = false;
    delete(sort_task);
    sort_task = NULL;
    }
}
void SortingAlgs::start(SORT_ALG alg) {
    if (thread_running || sort_task != NULL) {
        #ifdef DEBUG_SALG
        std::cout << "Stopping current thread in start" << std::endl;
        #endif
        stop();
    }
    switch(alg) {
        case SORT_ALG::INSERTION: 
            #ifdef DEBUG_SALG
            std::cout << "Starting Insertion Sort" << std::endl;
            #endif
            stop_flag->store(false, std::memory_order_relaxed);
            sort_task = new std::thread(threaded_insertion_sort, sr, SORTING_DELAY, stop_flag, paused, vectorAccessLock);
            thread_running = true;
            break;
        default:
            break;
    }
}


