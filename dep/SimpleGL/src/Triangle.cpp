#include "Triangle.h"

/*
 * Generate a triangle with width and height
 */
Triangle::Triangle(float width, float height) {
    this->width = width;
    this->height = height;

    float halfX = width/2;
    float halfY = height/2;
    
    // Generate default triangle x and y points (probably equilatoral)

    buffer[0] = -halfX;
    buffer[1] = -halfY;
    buffer[8] = halfX;
    buffer[9] = -halfY;
    buffer[13] = halfY;

    
    vao = VertexArray();
    vao.bind();
    vbo = BufferObject(GL_ARRAY_BUFFER);
    

}