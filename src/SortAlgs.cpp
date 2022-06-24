#include "SortAlgs.hpp"

void threaded_insertion_sort(SortingRects *sr, std::atomic<int> *delay, std::atomic<bool> *stop_flag,
                             std::atomic<bool> *pause, std::mutex *rectLock)
{
    for (int i = 0; i < (int)sr->rects.size(); i++)
    {
        SGLRect *key = sr->rects[i];
        int j = i;

        if (stop_flag->load(std::memory_order_relaxed))
        {
            return;
        }
        // PAUSE LOOP
        while (pause->load(std::memory_order_relaxed))
        {
            if (stop_flag->load(std::memory_order_relaxed))
                return;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        while (j > 0 && sr->rects[j - 1]->getHeight() > key->getHeight())
        {

            rectLock->lock();
            sr->rects[j] = sr->rects[j - 1];
            sr->rects[j]->setColor(0.6, 0.2, 0.2);
            sr->rects[j - 1]->setColor(0.2, 0.2, 0.6);
            rectLock->unlock();
            std::this_thread::sleep_for(std::chrono::microseconds(
                delay->load(std::memory_order_relaxed)));

            rectLock->lock();
            sr->resetColor(sr->rects[j - 1]);
            sr->resetColor(sr->rects[j]);
            rectLock->unlock();
            j--;
        }
        rectLock->lock();
        sr->rects[j] = key;
        rectLock->unlock();
        if (stop_flag->load(std::memory_order_relaxed))
        {
            return;
        }
        // PAUSE LOOP
        while (pause->load(std::memory_order_relaxed))
        {
            if (stop_flag->load(std::memory_order_relaxed))
                return;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
}

void threaded_merge_sort(SortingRects *const sr, std::atomic<int> *delay, std::atomic<bool> *stop_flag,
                         std::atomic<bool> *pause, std::mutex *rectLock, int l, int r)
{

    if (stop_flag->load(std::memory_order_relaxed))
        return;
    while (pause->load(std::memory_order_relaxed))
    {
        if (stop_flag->load(std::memory_order_relaxed))
            return;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    if (l >= r)
        return;

    // find middle
    int middle = l + (r - l) / 2; // floor(l+r/2)

    threaded_merge_sort(sr, delay, stop_flag, pause, rectLock, l, middle);
    if (stop_flag->load(std::memory_order_relaxed))
        return;
    while (pause->load(std::memory_order_relaxed))
    {
        if (stop_flag->load(std::memory_order_relaxed))
            return;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    threaded_merge_sort(sr, delay, stop_flag, pause, rectLock, middle + 1, r);

    // merge two halves;
    merge(sr, delay, stop_flag, pause, rectLock, l, middle, r);
    if (stop_flag->load(std::memory_order_relaxed))
        return;
    while (pause->load(std::memory_order_relaxed))
    {
        if (stop_flag->load(std::memory_order_relaxed))
            return;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void merge(SortingRects *const sr, std::atomic<int> *delay, std::atomic<bool> *stop_flag,
           std::atomic<bool> *pause, std::mutex *rectLock, int l, int m, int r)
{
    if (stop_flag->load(std::memory_order_relaxed))
        return;
    while (pause->load(std::memory_order_relaxed))
    {
        if (stop_flag->load(std::memory_order_relaxed))
            return;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    // sizes for new arrays
    int arrOne = m - l + 1;
    int arrTwo = r - m;

    // copy in sections of sr->rects into temp vectors

    rectLock->lock();
    std::vector<SGLRect *> leftArr(sr->rects.begin() + l, sr->rects.begin() + l + arrOne);
    std::vector<SGLRect *> rightArr(sr->rects.begin() + m + 1, sr->rects.begin() + m + 1 + arrTwo);

    // set their colors
    for (auto i = sr->rects.begin() + l; i != sr->rects.begin() + m + 1 + arrTwo; i++)
    {
        (*i)->setColor(0.3, 0.3, 1.0);
    }
    rectLock->unlock();

    int leftIndex = 0, rightIndex = 0;
    int mergedIndex = 0;

    // merge arrays

    while (leftIndex < arrOne && rightIndex < arrTwo)
    {
        // bad if we exit program here since we need to cleanup array, so we can skip delay if we do need to close
        if (!stop_flag->load(std::memory_order_relaxed))
        {
            std::this_thread::sleep_for(std::chrono::microseconds(
                delay->load(std::memory_order_relaxed)));
        }
        while (pause->load(std::memory_order_relaxed))
        {
            if (stop_flag->load(std::memory_order_relaxed))
                return;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        if (leftArr[leftIndex]->getHeight() <= rightArr[rightIndex]->getHeight())
        {
            rectLock->lock();
            sr->rects[l + mergedIndex] = leftArr[leftIndex];
            rectLock->unlock();
            leftIndex++;
            mergedIndex++;
        }
        else
        {
            rectLock->lock();
            sr->rects[l + mergedIndex] = rightArr[rightIndex];
            rectLock->unlock();
            rightIndex++;
            mergedIndex++;
        }
    }
    // copy in remaining

    while (rightIndex < arrTwo)
    {
        sr->rects[l + mergedIndex] = rightArr[rightIndex];
        rightIndex++;
        mergedIndex++;
    }
    while (leftIndex < arrOne)
    {
        sr->rects[l + mergedIndex] = leftArr[leftIndex];
        leftIndex++;
        mergedIndex++;
    }
    rectLock->lock();
    for (auto i = sr->rects.begin() + l; i != sr->rects.begin() + m + 1 + arrTwo; i++)
    {
        sr->resetColor(*i);
    }
    rectLock->unlock();
    if (stop_flag->load(std::memory_order_relaxed))
        return;
    while (pause->load(std::memory_order_relaxed))
    {
        if (stop_flag->load(std::memory_order_relaxed))
            return;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void heap_sort(SortingRects *const sr, std::atomic<int> *delay, std::atomic<bool> *stop_flag,
               std::atomic<bool> *pause, std::mutex *rectLock)
{
    size_t heapSize = sr->rects.size();
    buildMaxHeap(sr, delay, stop_flag, pause, rectLock);
    for (int i = heapSize - 1; i >= 0; i--)
    {
        PAUSESTOP()
        rectLock->lock();
        sr->rects[0]->setColor(0.1, 0.8, 0.2);
        sr->rects[i]->setColor(0.1, 0.8, 0.2);
        sr->swap(0, i);
        rectLock->unlock();

        std::this_thread::sleep_for(std::chrono::microseconds(
            delay->load(std::memory_order_relaxed)));

        rectLock->lock();
        sr->resetColor(sr->rects[0]);
        sr->resetColor(sr->rects[i]);
        rectLock->unlock();

        heapSize--;
        maxHeapify(sr, delay, stop_flag, pause, rectLock, 0, heapSize);
    }
}
void maxHeapify(SortingRects *const sr, std::atomic<int> *delay, std::atomic<bool> *stop_flag,
                std::atomic<bool> *pause, std::mutex *rectLock, const size_t root, const size_t heap_size)
{
    PAUSESTOP()
    size_t left = HEAP_LEFT(root);
    size_t right = HEAP_RIGHT(root);
    size_t largest = root;

    if (left < heap_size && sr->rects[left]->getHeight() > sr->rects[largest]->getHeight())
        largest = left;
    if (right < heap_size && sr->rects[right]->getHeight() > sr->rects[largest]->getHeight())
        largest = right;

    if (largest != root)
    {

        SGLRect *tmp = sr->rects[largest];
        SGLRect *tmp2 = sr->rects[root];

        rectLock->lock();
        tmp->setColor(0.1, 0.2, 0.7);
        tmp2->setColor(0.1, 0.2, 0.8);
        sr->swap(largest, root); // Swap for heapsort
        rectLock->unlock();

        std::this_thread::sleep_for(std::chrono::microseconds(
            delay->load(std::memory_order_relaxed)));

        maxHeapify(sr, delay, stop_flag, pause, rectLock, largest, heap_size);

        rectLock->lock();
        sr->resetColor(tmp);
        sr->resetColor(tmp2);
        rectLock->unlock();
    }
}
void buildMaxHeap(SortingRects *const sr, std::atomic<int> *delay, std::atomic<bool> *stop_flag,
                  std::atomic<bool> *pause, std::mutex *rectLock)
{
    const size_t heapSize = sr->rects.size();
    for (int i = heapSize / 2 - 1; i >= 0; i--)
    {
        maxHeapify(sr, delay, stop_flag, pause, rectLock, i, heapSize);
    }
}
void quickSort(SortingRects *const sr, std::atomic<int> *delay, std::atomic<bool> *stop_flag,
               std::atomic<bool> *pause, std::mutex *rectLock, int p, int r)
{
    PAUSESTOP();

    if (p < r)
    {
        std::this_thread::sleep_for(std::chrono::microseconds(
            delay->load(std::memory_order_relaxed)));
        int q = partition(sr, delay, stop_flag, pause, rectLock, p, r);
        quickSort(sr, delay, stop_flag, pause, rectLock, p, q - 1);
        quickSort(sr, delay, stop_flag, pause, rectLock, q + 1, r);
    }
}
int partition(SortingRects *const sr, std::atomic<int> *delay, std::atomic<bool> *stop_flag,
              std::atomic<bool> *pause, std::mutex *rectLock, int p, int r)
{
    SGLRect *x = sr->rects[r]; // pivot (we make it always the last element)
    rectLock->lock();
    sr->rects[r]->setColor(0.9, 0.4, 0.3);
    rectLock->unlock();
    int i = p - 1;
    for (int j = p; j < r; j++)
    {
        PAUSESTOP(0);
        std::this_thread::sleep_for(std::chrono::microseconds(
            delay->load(std::memory_order_relaxed)));
        rectLock->lock();
        sr->rects[j]->setColor(0.3, 0.4, 0.9);

        if ((*sr)[j] <= x->getHeight())
        {
            i++;
            sr->rects[i]->setColor(0.3, 0.9, 0.4);
            sr->swap(i, j);
        }
        rectLock->unlock();
    }
    rectLock->lock();
    for (int j = p; j < r; j++)
    {
        sr->resetColor(sr->rects[j]);
    }
    sr->resetColor(sr->rects[r]);
    rectLock->unlock();
    rectLock->lock();
    sr->swap(i + 1, r);
    rectLock->unlock();
    return i + 1;
}
SortingAlgs::SortingAlgs(SortingRects *sr, std::atomic<int> *delay, std::mutex *access_lock)
{
    this->sr = sr;
    this->vectorAccessLock = access_lock;
    this->SORTING_DELAY = delay;
    this->paused = new std::atomic<bool>(false);
    this->stop_flag = new std::atomic<bool>(false);
    this->sort_task = NULL;
}

SortingAlgs::~SortingAlgs()
{
    if (sort_task != NULL)
    {
        stop();
    }
    delete (paused);
    delete (stop_flag);
}

void SortingAlgs::pause()
{
    paused->store(true, std::memory_order_relaxed);
}
void SortingAlgs::unpause()
{
    paused->store(false, std::memory_order_relaxed);
}

void SortingAlgs::reset()
{
    stop();
    sr->randomize();
}

void SortingAlgs::stop()
{
    if (sort_task != NULL)
    {
        this->paused->store(false, std::memory_order_relaxed);
        this->stop_flag->store(true, std::memory_order_relaxed);

        // sleep for thread to join
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        if (sort_task->joinable())
        {
            sort_task->join();
        }
        thread_running = false;
        delete (sort_task);
        sort_task = NULL;
    }
}
void SortingAlgs::start(SORT_ALG alg)
{
    if (thread_running || sort_task != NULL)
    {
#ifdef DEBUG_SALG
        std::cout << "Stopping current thread in start" << std::endl;
#endif
        stop();
    }
    switch (alg)
    {
    case SORT_ALG::INSERTION:
#ifdef DEBUG_SALG
        std::cout << "Starting Insertion Sort" << std::endl;
#endif
        stop_flag->store(false, std::memory_order_relaxed);
        sort_task = new std::thread(threaded_insertion_sort, sr, SORTING_DELAY, stop_flag, paused, vectorAccessLock);
        thread_running = true;
        currentAlg = SORT_ALG::INSERTION;
        break;

    case SORT_ALG::MERGESORT:
#ifdef DEBUG_SALG
        std::cout << "Starting Merge Sort" << std::endl;
#endif
        stop_flag->store(false, std::memory_order_relaxed);
        sort_task = new std::thread(threaded_merge_sort, sr, SORTING_DELAY, stop_flag, paused, vectorAccessLock, 0, sr->rects.size() - 1);
        thread_running = true;
        currentAlg = SORT_ALG::MERGESORT;
        break;

    case SORT_ALG::HEAPSORT:
#ifdef DEBUG_SALG
        std::cout << "Starting Heap Sort" << std::endl;
#endif
        stop_flag->store(false, std::memory_order_relaxed);
        sort_task = new std::thread(heap_sort, sr, SORTING_DELAY, stop_flag, paused, vectorAccessLock);
        thread_running = true;
        currentAlg = SORT_ALG::HEAPSORT;
        break;
    case SORT_ALG::QUICKSORT:
#ifdef DEBUG_SALG
        std::cout << "Starting Quick Sort" << std::endl;
#endif
        stop_flag->store(false, std::memory_order_relaxed);
        sort_task = new std::thread(quickSort, sr, SORTING_DELAY, stop_flag, paused, vectorAccessLock, 0, sr->rects.size() - 1);
        thread_running = true;
        currentAlg = SORT_ALG::QUICKSORT;
        break;
    default:
        break;
    }
}
