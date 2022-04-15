#include <iostream>
#include <random>
#include <time.h>
#include <vector>

#include <glad/glad.h>  // Manages function pointers
#include <glfw3.h>      // Manages window
#include "SGLRect.hpp"

/*
 * Creates an array of rectangles you can sort by height
 */

class SortingRects {
private:
    std::vector<SGLRect*> rects;
    ShaderProgram* rShader = NULL;
public:
    SortingRects(int numRects, ShaderProgram* rShader);
    ~SortingRects();
    void draw();



};



