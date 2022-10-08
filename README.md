# Sorting-Algorithm-Visualizer
Visualize sorting algorithms in OpenGL

### Visualizing Algorithms
It is relatively easy to add more sorting algorithms to this project. Every algorithm runs on it's own separate thread and performs alterations on the rectangles we draw.
The only overhead we need to account for is delaying the algorithm so we can visualize it, other than that, all we need to do is sort the rectangles!



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
