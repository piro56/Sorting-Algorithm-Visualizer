#include "Triangle.hpp"

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
    vbo.bind();
    vbo.setBufferData(sizeof(buffer), this->buffer, GL_DYNAMIC_DRAW);
    // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 6 * sizeof(float), 0);
    glEnableVertexAttribArray(0);
    // Color
    glVertexAttribPointer(1, 3, GL_FLOAT, false, 6 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    
    this->transform = glm::mat4(1.0f);
}



void Triangle::draw() {
    vao.bind();
    glDrawArrays(GL_TRIANGLES, 0, 3);
}