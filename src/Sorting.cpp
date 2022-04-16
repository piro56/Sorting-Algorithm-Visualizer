#include "Sorting.hpp"

Sorting::Sorting(int numRects, ShaderProgram* rShader) {
    this->numRects = numRects;
    this->rShader = rShader;

    float* vertices = new float[numRects * 12];

    unsigned int* elements = new unsigned int[numRects * 6];
    std::fill(vertices, vertices + numRects * 4, 0.0f);

    float rectWidth = 2.0f / numRects;
    float halfW = rectWidth/ 2.0f;
    float rectHeight = 0.0f;

    // Generate rectangles
    
    for (int i = 0; i < numRects; i++) {
        rectHeight = -1.0 + 2.0f * ((float) rand() / RAND_MAX);
        float xOffL = -1 + i * rectWidth;
        float xOffR = -1 + ((i+1) * rectWidth);

        vertices[i * 12] = xOffL;         // BL x
        vertices[i * 12 + 1] = -1.0f;                   // BL y
        // 2 is Z
        vertices[i * 12 + 3] = xOffR;     // BR X
        vertices[i * 12 + 4] = -1.0f;                   // BR Y
        // 5 is Z
        vertices[i * 12 + 6] = xOffR;     // TR X
        vertices[i * 12 + 7] = rectHeight;              // TR Y
        // 8 is Z
        vertices[i * 12 + 9] = xOffL;     // TL X
        vertices[i * 12 + 10] = rectHeight;             // TR Y
        // 11 is Z
    }

    // Generate a EBO for every rect 
    // 2 rects 24, 8 VERTICES
    // 0, 1, 2, 3, 4, 5, 6, 7
    // 0 1 2 2 3 0
    // 4 5 6 6 8 4
    for (int i = 0; i < numRects; i++) {
        unsigned int idxOff = 4 * i;
        elements[i * 6] = idxOff;            // BL
        elements[i * 6 + 1] = idxOff + 1;    // BR
        elements[i * 6 + 2] = idxOff + 2;    // TR

        elements[i * 6 + 3] = idxOff + 2;    // TR
        elements[i * 6 + 4] = idxOff + 3;    // TL
        elements[i * 6 + 5] = idxOff;        // BL
    }


    vao.bind();
    vertBuffer.bind();
    vertBuffer.setBufferData(sizeof(float) * numRects * 12, vertices, GL_DYNAMIC_DRAW);
    elemBuffer.bind();
    elemBuffer.setBufferData(sizeof(unsigned int) * numRects * 6, elements, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(0);
    delete[] vertices;
    delete[] elements;
    elements = NULL;
    vertices = NULL;
}


void Sorting::draw() {
    vao.bind();
    elemBuffer.bind();
    glDrawElements(GL_TRIANGLES, 6 * numRects, GL_UNSIGNED_INT, 0);
}


Sorting::~Sorting() {
}