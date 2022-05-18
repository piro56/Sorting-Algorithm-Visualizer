#include "SortAlgs.hpp"


void threaded_insertion_sort(SortingRects* sr, std::atomic<int>* delay, std::atomic<bool>* stop_flag, 
                            std::atomic<bool>* pause, std::mutex* rectLock) {
    for (int i = 0; i < (int) sr->rects.size(); i++) {
        SGLRect* key = sr->rects[i];
        int j = i;
        
        if (stop_flag->load(std::memory_order_relaxed)) return;
        
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
                    delay->load(std::memory_order_relaxed) / 10));

            rectLock->lock();
            sr->resetColor(sr->rects[j-1]);
            sr->resetColor(sr->rects[j]);
            rectLock->unlock();
            j--;
        }
        rectLock->lock();
        sr->rects[j] = key;
        rectLock->unlock();
        if (stop_flag->load(std::memory_order_relaxed)) return;
        // PAUSE LOOP
        while (pause->load(std::memory_order_relaxed)) {
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
}

SortingAlgs::~SortingAlgs() {
    if (thread_running) {
        stop();
        sort_task.join();
    }
    delete(paused);
    delete(stop_flag);
}

void SortingAlgs::pause() {
    paused->store(true, std::memory_order_relaxed);
}

void SortingAlgs::reset() {
    pause();
    sort_task.join();
    sr->randomize();
}

void SortingAlgs::selectAlgorithm() {
    if (thread_running) {
        stop();
    }

}

void SortingAlgs::stop() {
    this->stop_flag->store(true, std::memory_order_relaxed);
    this->paused->store(false, std::memory_order_relaxed);
    sort_task.join();
    thread_running = false;
}
void SortingAlgs::start(SORT_ALG alg) {
    if (thread_running) {
        stop();
    }
    switch(alg) {
        case SORT_ALG::INSERTION: 
            sort_task = std::thread(threaded_insertion_sort, sr, SORTING_DELAY, stop_flag, paused, vectorAccessLock);
            thread_running = true;
            break;
        default:
            break;
    }
}


