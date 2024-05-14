#pragma once

#include "../misc/ray.h"
#include "../misc/color.h"
#include "../textures/texture.h"
#include "../utilities/randomizer.h"

// to avoid cyclic dependency
class hittable_list;
class hit_record;
class scatter_record;

/// <summary>
/// Abstract class for materials
/// </summary>
class material
{
public:
    material();
    material(std::shared_ptr<texture> _diffuse);
    material(std::shared_ptr<texture> _diffuse, std::shared_ptr<texture> _specular);
    material(std::shared_ptr<texture> _diffuse, std::shared_ptr<texture> _specular, std::shared_ptr<texture> _normal, double transparency, double refractive_index);
    virtual ~material() = default;

    virtual bool scatter(const ray& r_in, const hittable_list& lights, const hit_record& rec, scatter_record& srec, randomizer& random) const;
    virtual double scattering_pdf(const ray& r_in, const hit_record& rec, const ray& scattered) const;
    virtual color emitted(const ray& r_in, const hit_record& rec, double u, double v, const point3& p) const;

protected:

    std::shared_ptr<texture> m_diffuse_texture = nullptr;
    std::shared_ptr<texture> m_specular_texture = nullptr;
    std::shared_ptr<texture> m_normal_texture = nullptr;

    //bool m_isTransparent = false;
    double m_refractiveIndex = 0.0;
    double m_transparency = 0.0;

    //double m_reflectivity = 0;
    //double m_transparency = 0;
    //double m_refractiveIndex = 0;
    //double m_specularity = 0;
    //double m_specularExponent = 0;
    //double m_emissivity = 0;
    //double m_roughness = 0;
};