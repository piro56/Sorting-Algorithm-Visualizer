#include "SortingRects.hpp"

SortingRects::SortingRects(int numRects, ShaderProgram* rShader) {
    this->rShader = rShader;
    this->rects.reserve(numRects);

    float recWidths = 2.0f / numRects; 
    float offset = -1.0f + recWidths * 0.5;
    for (int i = 0; i < numRects; i++) {
        // generates height 0 to 1
        float rheight = (float) rand() / RAND_MAX;
        rects.emplace_back(new SGLRect(recWidths, rheight, rShader));
        float color = std::min(1.0f, rheight + 0.1f);
        rects.back()->setColor(color, color, color);

        // -1.0 + width*i <- aligns rects left to right
        // -1.0 + 0.5 * rheight <- aligns bottom of rectangles to bottom of screen
        rects.back()->setPosition(offset + recWidths * i, -1.0 + 0.5 * rheight);
    }
}
SortingRects::~SortingRects() {
    for (auto it = rects.begin(); it != rects.end(); it++) {
        delete *it;
        *it = NULL;
    }
}

void SortingRects::draw() {
    for (SGLRect* sr : rects) {
        sr->draw();
    }
}