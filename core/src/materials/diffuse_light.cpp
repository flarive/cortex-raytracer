#include "diffuse_light.h"

#include "../textures/solid_color_texture.h"

diffuse_light::diffuse_light(std::shared_ptr<texture> a)
    : m_emit(a) {}

diffuse_light::diffuse_light(color _c)
    : m_emit(std::make_shared<solid_color_texture>(_c)), m_intensity(1.0), m_invisible(true), m_directional(true) {}

diffuse_light::diffuse_light(color _c, double _intensity, bool _directional, bool _invisible)
    : m_emit(std::make_shared<solid_color_texture>(_c)), m_intensity(_intensity), m_directional(_directional), m_invisible(_invisible)
{
}

color diffuse_light::emitted(const ray& r_in, const hit_record& rec, double u, double v, const point3& p) const
{
    // Material emission, directional or not
    if (m_directional)
    {
        if (rec.front_face)
        {
            // light
            return m_emit->value(u, v, p) * m_intensity;
        }
        else
        {
            // no light
            return color(0, 0, 0, 0);
        }
    }
    else
    {
        // light
        return m_emit->value(u, v, p) * m_intensity;
    }

    return m_emit->value(u, v, p) * m_intensity;
}