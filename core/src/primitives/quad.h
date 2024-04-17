#pragma once

#include "../constants.h"
#include "hittable.h"
#include "hittable_list.h"
#include "../misc/ray.h"
#include "../misc/hit_record.h"

#include <glm/glm.hpp>
#include "../utilities/types.h"

/// <summary>
/// Quadrilateral (plane) primitive
/// </summary>
class quad : public hittable
{
public:
    quad(const point3& _Q, const vector3& _u, const vector3& _v, std::shared_ptr<material> m, std::string _name = "Quad");

    void set_bounding_box();

    aabb bounding_box() const override;


    bool hit(const ray& r, interval ray_t, hit_record& rec, int depth) const override;
    

    bool is_interior(double a, double b, hit_record& rec) const;


    double pdf_value(const point3& origin, const vector3& v) const override;


    /// <summary>
    /// Random special implementation for quad (override base)
    /// </summary>
    /// <param name="origin"></param>
    /// <returns></returns>
    vector3 random(const point3& origin) const override;


    /// <summary>
    /// Update the internal AABB of the mesh.
    /// Warning: run this when the mesh is updated.
    /// </summary>
    void updateBoundingBox() override;



private:
    point3 Q; // the lower-left corner
    vector3 u; // a vector representing the first side
    vector3 v; //  a vector representing the second side
    std::shared_ptr<material> mat;
    //aabb bbox;
    vector3 normal;
    double D;
    vector3 w; // The vector w is constant for a given quadrilateral, so we'll cache that value
    double area;
};