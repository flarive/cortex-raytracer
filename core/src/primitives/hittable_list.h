#pragma once

#include "../aabb.h"
#include "../misc/hit_record.h"
#include "../misc/ray.h"
#include "../utilities/types.h"
#include "../utilities/math_utils.h"
#include "../utilities/interval.h"
#include "hittable.h"

#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

/// <summary>
/// List of hittable primitives
/// </summary>
class hittable_list : public hittable
{
public:
    std::vector<shared_ptr<hittable>> objects;

    hittable_list(std::string _name = "HittableList")
    {
        name = _name;
    }

    hittable_list(shared_ptr<hittable> object, std::string _name = "HittableList")
    {
        name = _name;

        add(object);
    }

    void clear();


    void add(shared_ptr<hittable> object);

    bool hit(const ray& r, interval ray_t, hit_record& rec, int depth) const override;


    aabb bounding_box() const override;


    double pdf_value(const point3& o, const vector3& v) const override;


    /// <summary>
    /// Random special implementation for hittable list (override base)
    /// </summary>
    /// <param name="origin"></param>
    /// <returns></returns>
    vector3 random(const vector3& o) const override;



private:
    aabb bbox;

    //// <summary>
    /// Update the internal AABB of the mesh.
    /// Warning: run this when the mesh is updated.
    /// </summary>
    void updateBoundingBox() override;
};