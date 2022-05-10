#include "SortingRects.hpp"

SortingRects::SortingRects(int numRects, ShaderProgram* rShader) {
    this->rShader = rShader;
    this->rects.reserve(numRects);

    recWidths = 2.0f / numRects; 
    wOffset = -1.0f + recWidths * 0.5;
    for (int i = 0; i < numRects; i++) {
        // generates height 0 to 1
        float rheight = (float) rand() / RAND_MAX;
        rects.emplace_back(new SGLRect(recWidths, rheight, rShader));
        //float color = std::min(1.0f, rheight + 0.1f);
        rects.back()->setColor(rheight + 0.2, rheight + 0.2, rheight + 0.2);

        // -1.0 + width*i <- aligns rects left to right
        // -1.0 + 0.5 * rheight <- aligns bottom of rectangles to bottom of screen
        rects.back()->setPosition(wOffset + recWidths * i, -1.0 + 0.5 * rheight);
    }
}
SortingRects::~SortingRects() {
    for (auto it = rects.begin(); it != rects.end(); it++) {
        delete *it;
        *it = NULL;
    }
}

void SortingRects::draw() {
    for (size_t i = 0; i < rects.size(); i++) {
        rects[i]->setPosition(wOffset + recWidths * i, -1.0 + 0.5 * rects[i]->getHeight());
        rects[i]->draw();
    }
}
void SortingRects::resetColor(SGLRect* rect) {
    float col = rect->getHeight() + 0.2;
    rect->setColor(col, col, col);
}

void SortingRects::printHeights() {
    for (size_t i = 1; i < rects.size() + 1; i++) {
        std::cout << rects[i-1]->getHeight() << "  ";
        if (i % 10 == 0) {
            std::cout << std::endl;
        }
    }
}