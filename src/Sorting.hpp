#include <iostream>
#include <random>
#include <time.h>
#include <vector>

#include <glad/glad.h>  // Manages function pointers
#include <glfw3.h>      // Manages window
#include "SGLRect.hpp"
#include "ShaderProgram.hpp"
#include "BufferObject.hpp"
#include "VertexArray.hpp"

struct Point {
    float x;
    float y;
    float z;
};

struct Rect {
    Point BL;
    Point BR;
    Point TR;
    Point TL;
};


/*
 * Creates an array of rectangles you can sort by height
 */

class Sorting {
private:
    VertexArray vao;
    BufferObject vertBuffer = BufferObject(GL_ARRAY_BUFFER);
    //BufferObject colBuffer = BufferObject(GL_ARRAY_BUFFER);
    BufferObject elemBuffer = BufferObject(GL_ELEMENT_ARRAY_BUFFER);

    ShaderProgram* rShader = NULL;
    float recWidths;
    float wOffset;
    int numRects;



public:
    // holds all of the vertices for the rectangles
    Rect* rectangles;

    /* updates vertBuffer to match data in vertices */
    void update_buffer();

    /* Creates numRects amount of rectangles and fills them in vector rects *
     * Sets fixed width and a random height to each rectangle               */
    Sorting(int numRects, ShaderProgram* rShader);
    
    /* Deallocates rects */
    ~Sorting();

    /* Issues draw call on each SGLRect. Does not bind shader program.*/
    void draw();
    /* Sets rectangle back to its original shade */
    void resetColor(SGLRect* rect);

    void insertionSort();

    /* Print for debug*/
    void printPoints();

    void swapRand();


};



