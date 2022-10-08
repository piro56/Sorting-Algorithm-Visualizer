# Sorting-Algorithm-Visualizer
**Easily visualize sorting algorithms in OpenGL** <br/>
Run sorting functions on separate thread and perform alterations based off rectangle height. <br/>
Very easy to add more sorting algorithms to this project! <br/>

Currently have implemented:
- Insertion Sort
- Merge Sort
- Heap Sort
- Quick Sort
<br/>

![sort2](https://user-images.githubusercontent.com/69094266/194721122-fc3f53b8-faf0-479e-9e8a-f518e16c802f.gif)

Adding your own sorting algorithms:

- atomic<int> delay: Sorting delay time which changes with speed slider
- atomic<bool> stop_flag: Indicates premature quit of this thread. Use it to cleanup and exit your sorting algorithm prematurely.
- atomic<bool> pause: Use this to spin in your algorithm when pause button is pressed
- mutex rectLock: Lock this mutex when performing any swaps/array operations to avoid race conditions
- SortingRects sr: Holds the rectangles we want to draw, sort the array: 'rects' in this class to visualize output.

Then finally, add your sorting function to "start" in SortingAlgs class.


## Learning Outcomes:
- Gain some familiarity with CMake
- Implement and learn sorting algorithms
- Multithreading
