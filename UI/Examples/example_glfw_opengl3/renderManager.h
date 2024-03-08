#pragma once

#include "plotPixel.h"

#include <string>
#include <map>
#include <vector>

class renderManager
{

public:

    renderManager();
    void initFromWidth(unsigned int _width, double _aspectRatio);
    void initFromHeight(unsigned int _height, double _aspectRatio);

    void addPixelToFrameBuffer(unsigned int x, unsigned int y, unsigned int r, unsigned int g, unsigned int b, unsigned int a);
    unsigned char* getFrameBuffer();
    unsigned long getFrameBufferSize();

    void addPixel(unsigned int index, plotPixel* plotPixel);

    plotPixel* parsePixelEntry(unsigned int _index, std::string _entry);

    bool isFullyRendered();

    void renderAll();
    void renderLine(unsigned int y);

    void clearFrameBuffer(bool clearBuffer = true);

    unsigned int getWidth();
    unsigned int getHeight();

    float getRenderProgress();


private:
    unsigned int width = 0;
    unsigned int height = 0;
    double ratio = 0.0;

    unsigned char* buffer = nullptr;

    std::map<unsigned int, plotPixel> pixels;
    std::map<unsigned int, bool> drawn;
};
