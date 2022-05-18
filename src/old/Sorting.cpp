#include "Sorting.hpp"

Sorting::Sorting(int numRects, ShaderProgram* rShader) {
    this->numRects = numRects;
    this->rShader = rShader;

    float* vertices = new float[numRects * 12];

    unsigned int* elements = new unsigned int[numRects * 6];
    std::fill(vertices, vertices + numRects * 4, 0.0f);

    float rectWidth = 2.0f / numRects;
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
    
    this->rectangles = (Rect*) vertices;

    // Generate a EBO for every rect 
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
    vertBuffer.setBufferData(sizeof(float) * numRects * 12, NULL, GL_STREAM_DRAW);
    void* buffer = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    memcpy(buffer, vertices, sizeof(float) * numRects * 12);
    glUnmapBuffer(GL_ARRAY_BUFFER);
    elemBuffer.bind();
    elemBuffer.setBufferData(sizeof(unsigned int) * numRects * 6, elements, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(0);
    delete[] elements;
    elements = NULL;

}

void Sorting::draw() {
    vao.bind();
    elemBuffer.bind();
    glDrawElements(GL_TRIANGLES, 6 * numRects, GL_UNSIGNED_INT, 0);
}

void printPoint(Point& p) {
    std::cout << "(" << p.x << ", " << p.y << ", " << p.z << ") ";
}

void Sorting::printPoints() {
    for (int i = 0; i < numRects; i++) {
        printPoint(rectangles[i].BL);
        std::cout << "  ";
        printPoint(rectangles[i].BR);
        std::cout << "  ";
        printPoint(rectangles[i].TR);
        std::cout << "  ";
        printPoint(rectangles[i].TL);
        std::cout << std::endl;
    }
}

void Sorting::update_buffer() {
    vertBuffer.setBufferData(sizeof(float) * numRects * 12, NULL, GL_STREAM_DRAW);
    void* buffer = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    memcpy((float*) rectangles, buffer, sizeof(float) * numRects * 12);
    glUnmapBuffer(GL_ARRAY_BUFFER);
}

void Sorting::insertionSort() {
    for (int i = 0; i < (int) numRects; i++) {
    Rect key = rectangles[i];
    int j = i;
    while (j > 0 && rectangles[j-1].TR.y > key.TR.y) {
        rectangles[j] = rectangles[j-1];
        j--;
    }
    rectangles[j] = key;
    }
}

Sorting::~Sorting() {
    delete[] rectangles;
}