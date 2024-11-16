#pragma once

#include "../materials/material.h"

class emissive_material : public material
{
public:
    emissive_material(std::shared_ptr<texture> a);
    emissive_material(std::shared_ptr<texture> a, double _intensity);


    emissive_material(color _c);
    emissive_material(color _c, double _intensity);

    color emitted(const ray& r_in, const hit_record& rec, double u, double v, const point3& p) const override;


private:
    std::shared_ptr<texture> m_emit = nullptr;
    double m_intensity = 1.0;
    bool m_directional = true;
    bool m_invisible = true;
};

