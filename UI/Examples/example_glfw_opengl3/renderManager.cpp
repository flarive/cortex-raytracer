#include "renderManager.h"



#include <algorithm>
#include <string>
#include <vector>
#include <sstream>
#include <stdexcept>
#include <iostream>


using namespace std;


renderManager::renderManager()
{

}

void renderManager::initFromWidth(unsigned int _width, double _ratio)
{
    width = _width;
    height = static_cast<int>(_width / _ratio);
    ratio = _ratio;

    clearFrameBuffer();

    // init render buffer
    buffer = new unsigned char[width * height * 4];
}

void renderManager::initFromHeight(unsigned int _height, double _ratio)
{
    height = _height;
    width = static_cast<int>(_height * _ratio);
    ratio = _ratio;

    clearFrameBuffer();

    // init render buffer
    buffer = new unsigned char[width * height * 4];
}

unsigned char* renderManager::getFrameBuffer()
{
    if (buffer == nullptr)
    {
        buffer = new unsigned char[0];
    }

    return buffer;
}

unsigned long renderManager::getFrameBufferSize()
{
    return static_cast<long>(pixels.size());
}

void renderManager::clearFrameBuffer()
{
    buffer = nullptr;
    pixels.clear();
    drawn.clear();
}

unsigned int renderManager::getWidth()
{
    return width;
}

unsigned int renderManager::getHeight()
{
    return height;
}

float renderManager::getRenderProgress()
{
    if (width <= 0 || height <= 0 || drawn.size() == 0)
    {
        return 0.0;
    }

    return (((float)drawn.size() * 100) / (float)(width * height)) / 100;
}

void renderManager::addPixel(unsigned int index, plotPixel *plotPixel)
{
    if (plotPixel->y < height && plotPixel->x < width)
    {
        pixels.emplace(index, *plotPixel);
        //cout << plotPixel->x << " " << plotPixel->y << " : " << plotPixel->r << " " << plotPixel->g << " " << plotPixel->b << "\n";
    }
}


void renderManager::addPixelToFrameBuffer(unsigned int x, unsigned int y, unsigned int r, unsigned int g, unsigned int b, unsigned int a)
{
    // revert drawing order vertical
    auto fixedY = height - 1 - y;

    buffer[4 * (fixedY * width + x) + 0] = r;
    buffer[4 * (fixedY * width + x) + 1] = g;
    buffer[4 * (fixedY * width + x) + 2] = b;
    buffer[4 * (fixedY * width + x) + 3] = a;
}


bool renderManager::isFullyRendered()
{
    if (drawn.size() < ((width * height) - 1))
    {
        return false;
    }

    return true;
}

void renderManager::renderAll()
{
    if (pixels.size() <= 0)
    {
        return;
    }

    if (isFullyRendered())
    {
        //cout << "Finished !!!!\n";
        return;
    }

    for (unsigned int y = 0; y < height; ++y)
    {
        for (unsigned int x = 0; x < width; ++x)
        {
            unsigned int index = (y * width) + x;
            //cout << "(" << y << "* " << width << ") + " << x << " >>> index " << index << "\n !!!!";

            if (drawn.find(index) == drawn.end())
            {
                /* v does not contain x */
                if (pixels.find(index) != pixels.end())
                {
                    // found
                    plotPixel pixel;
                    try
                    {
                        pixel = pixels.at(index);
                    }
                    catch (const out_of_range& e)
                    {
                        cerr << e.what() << std::endl;

                        // draw debug red pixel
                        pixel.x = x;
                        pixel.y = y;
                        pixel.r = 255;
                        pixel.g = 0;
                        pixel.b = 0;
                        pixel.a = 255;
                    }

                    addPixelToFrameBuffer(pixel.x, pixel.y, pixel.r, pixel.g, pixel.b, 255);
                    drawn.emplace(index, true);
                }
            }
        }
    }
}

void renderManager::renderLine(unsigned int y)
{
    if (pixels.size() <= 0)
    {
        return;
    }

    if (isFullyRendered())
    {
        //cout << "Finished !!!!\n";
        return;
    }

    for (unsigned int x = 0; x < width; ++x)
    {
        unsigned int index = (y * width) + x;
        //cout << "(" << y << "* " << width << ") + " << x << " >>> index " << index << "\n !!!!";

        if (drawn.find(index) == drawn.end())
        {
            /* v does not contain x */
            if (pixels.find(index) != pixels.end())
            {
                // found
                plotPixel pixel;
                try
                {
                    pixel = pixels.at(index);
                }
                catch (const out_of_range& e)
                {
                    cerr << e.what() << std::endl;

                    // draw debug red pixel
                    pixel.x = x;
                    pixel.y = y;
                    pixel.r = 255;
                    pixel.g = 0;
                    pixel.b = 0;
                    pixel.a = 255;
                }

                addPixelToFrameBuffer(pixel.x, pixel.y, pixel.r, pixel.g, pixel.b, 255);
                drawn.emplace(index, true);
            }
        }
    }
}

plotPixel* renderManager::parsePixelEntry(unsigned int _index, string _entry)
{
    if (_entry.empty())
    {
        // return black pixel
        return nullptr;
    }

    auto count = ranges::count(_entry, ' ');
    if (count == 2)
    {
        plotPixel* pixel = new plotPixel();

        stringstream test(_entry);
        string segment;

        unsigned int loop = 0;
        while (getline(test, segment, ' '))
        {
            if (loop == 0)
            {
                // r
                if (!segment.empty())
                {
                    unsigned long lresult = stoul(segment, 0, 10);
                    pixel->r = lresult;
                }
            }
            else if (loop == 1)
            {
                // g
                if (!segment.empty())
                {
                    unsigned long lresult = stoul(segment, 0, 10);
                    pixel->g = lresult;
                }
            }
            else if (loop == 2)
            {
                // b
                if (!segment.empty())
                {
                    unsigned long lresult = stoul(segment, 0, 10);
                    pixel->b = lresult;
                }
            }

            loop++;
        }

        pixel->x = _index % width;
        pixel->y = _index / width;

        return pixel;
    }

    return nullptr;
}
