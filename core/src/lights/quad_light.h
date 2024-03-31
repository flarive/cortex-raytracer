#pragma once

#include "light.h"
#include "../primitives/hittable.h"
#include "../misc/color.h"
#include "../materials/diffuse_light.h"
#include "../utilities/math_utils.h"
#include "../renderParameters.h"
#include "../singleton.h"


/// <summary>
/// Quad light
/// </summary>
class quad_light : public light
{
public:
    quad_light(const point3& _Q, const vector3& _u, const vector3& _v, double _intensity, color _color, std::string _name = "QuadLight", bool _invisible = true);
    
    void set_bounding_box();
    aabb bounding_box() const override;
    point3 getPosition() const override;
    bool hit(const ray& r, interval ray_t, hit_record& rec, int depth) const override;
    bool is_interior(double a, double b, hit_record& rec) const;
    double pdf_value(const point3& origin, const vector3& v) const override;

    /// <summary>
    /// Random special implementation for quad light (override base)
    /// </summary>
    /// <param name="origin"></param>
    /// <returns></returns>
    vector3 random(const point3& origin) const override;


private:
    point3 Q; // the lower-left corner
    vector3 u; // vector representing the first side of the quadrilateral
    vector3 v; //  vector representing the second side of the quadrilateral
    vector3 normal; // vector representing quadrilateral normal
    double D;
    vector3 w; // vector w is constant for a given quadrilateral, so we'll cache that value
    double area;
};