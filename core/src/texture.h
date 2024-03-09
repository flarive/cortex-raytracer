#ifndef TEXTURE_H
#define TEXTURE_H

#include "constants.h"
#include "color.h"
#include "point3.h"
#include "stb_image.h"
#include "perlin.h"


/// <summary>
/// Texture base class
/// </summary>
class texture
{
public:
    virtual ~texture() = default;

    virtual color value(double u, double v, const point3& p) const = 0;
};


/// <summary>
/// Solid color texture
/// </summary>
class solid_color : public texture 
{
public:
    solid_color(color c) : color_value(c) {}

    solid_color(double red, double green, double blue) : solid_color(color(red, green, blue)) {}

    color value(double u, double v, const point3& p) const override
    {
        return color_value;
    }

private:
    color color_value;
};


/// <summary>
/// Checker texture
/// </summary>
class checker_texture : public texture
{
public:
    checker_texture(double _scale, shared_ptr<texture> _even, shared_ptr<texture> _odd)
        : inv_scale(1.0 / _scale), even(_even), odd(_odd) {}

    checker_texture(double _scale, color c1, color c2)
        : inv_scale(1.0 / _scale),
        even(make_shared<solid_color>(c1)),
        odd(make_shared<solid_color>(c2))
    {}

    color value(double u, double v, const point3& p) const override
    {
        auto xInteger = static_cast<int>(std::floor(inv_scale * p.x()));
        auto yInteger = static_cast<int>(std::floor(inv_scale * p.y()));
        auto zInteger = static_cast<int>(std::floor(inv_scale * p.z()));

        bool isEven = (xInteger + yInteger + zInteger) % 2 == 0;

        return isEven ? even->value(u, v, p) : odd->value(u, v, p);
    }

private:
    double inv_scale;
    shared_ptr<texture> even;
    shared_ptr<texture> odd;
};


/// <summary>
/// Image texture
/// </summary>
class image_texture : public texture
{
public:
    image_texture(const char* filename) : image(filename) {}

    color value(double u, double v, const point3& p) const override
    {
        // If we have no texture data, then return solid cyan as a debugging aid.
        if (image.height() <= 0) return color(0, 1, 1);

        // Clamp input texture coordinates to [0,1] x [1,0]
        u = interval(0, 1).clamp(u);
        v = 1.0 - interval(0, 1).clamp(v);  // Flip V to image coordinates

        auto i = static_cast<int>(u * image.width());
        auto j = static_cast<int>(v * image.height());
        auto pixel = image.pixel_data(i, j);

        auto color_scale = 1.0 / 255.0;
        return color(color_scale * pixel[0], color_scale * pixel[1], color_scale * pixel[2]);
    }

private:
    stb_image image;
};


/// <summary>
/// Perlin noise texture
/// </summary>
class noise_texture : public texture
{
public:
    noise_texture() {}

    noise_texture(double sc) : scale(sc) {}

    color value(double u, double v, const point3& p) const override
    {
        auto s = scale * p;
        return color(1, 1, 1) * 0.5 * (1 + sin(s.z() + 10 * noise.turb(s)));
    }

private:
    perlin noise;
    double scale = 0.0;
};

#endif