#include "image_texture.h"

image_texture::image_texture(const std::string filepath) : m_image(filepath.c_str())
{
}


color image_texture::value(double u, double v, const point3& p) const
{
    // If we have no texture data, then return solid cyan as a debugging aid.
    if (m_image.height() <= 0) return color(0, 1, 1);

    // Clamp input texture coordinates to [0,1] x [1,0]
    u = interval(0, 1).clamp(u);
    v = 1.0 - interval(0, 1).clamp(v);  // Flip V to image coordinates

    auto i = static_cast<int>(u * m_image.width());
    auto j = static_cast<int>(v * m_image.height());
    auto pixel = m_image.pixel_data(i, j);

    auto color_scale = 1.0 / 255.0;
    return color(color_scale * pixel[0], color_scale * pixel[1], color_scale * pixel[2]);
}

int image_texture::getWidth() const
{
    return m_image.width();
}

int image_texture::getHeight() const
{
    return m_image.height();
}