# Sorting-Algorithm-Visualizer
**Visualizes Sorting Algorithms in OpenGL** <br/>
Allows quick visualization to sorting algorithms with just minor adjustments.
Quick Steps to allow visualization:
1. Set delays
2. Add places algorithm is safe to stop with stop_flag (perform cleanup)
3. Add places algorithm is safe to pause with pause_flag
4. Add lock whenever performing a write
```cpp
- atomic<int> delay: Sorting delay time which changes with speed slider
- atomic<bool> stop_flag: Indicates premature quit of this thread. Use it to cleanup and exit your sorting algorithm prematurely.
- atomic<bool> pause: Use this to spin in your algorithm when pause button is pressed
- mutex rectLock: Lock this mutex when performing any swaps/array operations to avoid race conditions
- SortingRects sr: Holds the rectangles we want to draw, sort the array: 'rects' in this class to visualize output.
```
<br/>

![sort2](https://user-images.githubusercontent.com/69094266/194721122-fc3f53b8-faf0-479e-9e8a-f518e16c802f.gif)

## Learning Outcomes:
- Gain familiarity with CMake
- Implement and learn sorting algorithms
- Multithreading
