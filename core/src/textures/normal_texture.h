#pragma once

#include "../utilities/types.h"
#include "../misc/color.h"
#include "texture.h"

/// <summary>
/// Image texture
/// </summary>
class normal_texture : public texture
{
public:
    /// <summary>
    /// Strength can be between -1.0 (texture inverted normal), 0.0 (no normal) and 1.0 (texture max normal)
    /// </summary>
    /// <param name="normal"></param>
    /// <param name="strength"></param>
    normal_texture(std::shared_ptr<texture> normal, double strength = 1.0);

    color value(double u, double v, const point3& p) const override;

    double getStrenth();
private:
    std::shared_ptr<texture> m_normal = nullptr;
    double m_strength = 10.0;
};