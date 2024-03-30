#pragma once

#include "../misc/ray.h"
#include "../misc/color.h"

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
    virtual ~material() = default;

    virtual bool scatter(const ray& r_in, const hittable_list& lights, const hit_record& rec, scatter_record& srec) const;
    virtual double scattering_pdf(const ray& r_in, const hit_record& rec, const ray& scattered) const;
    virtual color emitted(const ray& r_in, const hit_record& rec, double u, double v, const point3& p) const;
};