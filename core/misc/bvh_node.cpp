#include "bvh_node.h"

#include "singleton.h"

#include <algorithm>
#include <execution>
#include <numeric>

bvh_node::bvh_node(const hittable_list& list, randomizer& rnd, std::string name)
    : bvh_node(list.objects, 0, list.objects.size(), rnd, name)
{
}

bvh_node::bvh_node(const std::vector<std::shared_ptr<hittable>>& src_objects, size_t start, size_t end, randomizer& rnd, std::string name)
{
    setName(name);

    std::vector<std::shared_ptr<hittable>> objects(src_objects.begin() + start, src_objects.begin() + end);
    size_t object_span = end - start;

    if (object_span == 1)
    {
        m_left = m_right = objects[0];
    }
    else if (object_span == 2)
    {
        if (box_compare(objects[0], objects[1], rnd.get_int(0, 2))) {
            m_left = objects[0];
            m_right = objects[1];
        }
        else {
            m_left = objects[1];
            m_right = objects[0];
        }
    }
    else
    {
        int axis = rnd.get_int(0, 2);

        auto comparator = [axis](const std::shared_ptr<hittable>& a, const std::shared_ptr<hittable>& b)
        {
            return box_compare(a, b, axis);
        };

        // Using parallel sort to speed up the sorting process
        std::sort(std::execution::par, objects.begin(), objects.end(), comparator);

        size_t mid = object_span / 2;
        m_left = std::make_shared<bvh_node>(objects, 0, mid, rnd, name + "_left");
        m_right = std::make_shared<bvh_node>(objects, mid, object_span, rnd, name + "_right");
    }

    m_bbox = aabb(m_left->bounding_box(), m_right->bounding_box());
}

bool bvh_node::hit(const ray& r, interval ray_t, hit_record& rec, int depth, randomizer& rnd) const
{
    if (!m_bbox.hit(r, ray_t))
        return false;

    bool hit_left = m_left->hit(r, ray_t, rec, depth, rnd);
    bool hit_right = m_right->hit(r, interval(ray_t.min, hit_left ? rec.t : ray_t.max), rec, depth, rnd);

    return hit_left || hit_right;
}

aabb bvh_node::bounding_box() const
{
    return m_bbox;
}

/// <summary>
/// Update the internal AABB of the mesh.
/// Warning: run this when the mesh is updated.
/// </summary>
void bvh_node::updateBoundingBox()
{
    // to implement
}

bool bvh_node::box_compare(const std::shared_ptr<hittable> a, const std::shared_ptr<hittable> b, int axis_index)
{
    return a->bounding_box().axis(axis_index).min < b->bounding_box().axis(axis_index).min;
}

bool bvh_node::box_x_compare(const std::shared_ptr<hittable> a, const std::shared_ptr<hittable> b)
{
    return box_compare(a, b, 0);
}

bool bvh_node::box_y_compare(const std::shared_ptr<hittable> a, const std::shared_ptr<hittable> b)
{
    return box_compare(a, b, 1);
}

bool bvh_node::box_z_compare(const std::shared_ptr<hittable> a, const std::shared_ptr<hittable> b)
{
    return box_compare(a, b, 2);
}