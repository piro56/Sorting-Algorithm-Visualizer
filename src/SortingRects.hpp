#include <iostream>
#include <random>
#include <time.h>
#include <vector>

#include <glad/glad.h>  // Manages function pointers
#include <glfw3.h>      // Manages window
#include "SGLRect.hpp"
#include "ShaderProgram.hpp"


/*
 * Creates an array of rectangles you can sort by height
 */

class SortingRects {
private:
    ShaderProgram* rShader = NULL;
    float recWidths;
    float wOffset;
public:
    
    /* Public for easier use to swap and display sorting */
    std::vector<SGLRect*> rects;

    /* Creates numRects amount of rectangles and fills them in vector rects *
     * Sets fixed width and a random height to each rectangle               */
    SortingRects(int numRects, ShaderProgram* rShader);
    
    /* Deallocates rects */
    ~SortingRects();

    /* Issues draw call on each SGLRect. Does not bind shader program.*/
    void draw();
    /* Sets rectangle back to its original shade */
    void resetColor(SGLRect* rect);

    /* Print for debug*/
    void printHeights();

};



