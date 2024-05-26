#include "bvh_node.h"

#include "utilities/randomizer.h"

#include <algorithm>

bvh_node::bvh_node(const hittable_list& list, std::string name)
    : bvh_node(list.objects, 0, list.objects.size(), name)
{
}

bvh_node::bvh_node(const std::vector<std::shared_ptr<hittable>>& src_objects, size_t start, size_t end, std::string name)
{
    auto objects = src_objects; // Create a modifiable array of the source scene objects

    m_name = name;

    int axis = randomizer::random_int(0, 2);
    auto comparator = (axis == 0) ? box_x_compare
        : (axis == 1) ? box_y_compare
        : box_z_compare;

    size_t object_span = end - start;

    if (object_span == 1) {
        m_left = m_right = objects[start];
    }
    else if (object_span == 2) {
        if (comparator(objects[start], objects[start + 1])) {
            m_left = objects[start];
            m_right = objects[start + 1];
        }
        else {
            m_left = objects[start + 1];
            m_right = objects[start];
        }
    }
    else {
        std::sort(objects.begin() + start, objects.begin() + end, comparator);

        auto mid = start + object_span / 2;
        m_left = make_shared<bvh_node>(objects, start, mid);
        m_right = make_shared<bvh_node>(objects, mid, end);
    }

    m_bbox = aabb(m_left->bounding_box(), m_right->bounding_box());
}



bool bvh_node::hit(const ray& r, interval ray_t, hit_record& rec, int depth) const
{
    if (!m_bbox.hit(r, ray_t))
        return false;

    bool hit_left = m_left->hit(r, ray_t, rec, depth);
    bool hit_right = m_right->hit(r, interval(ray_t.min, hit_left ? rec.t : ray_t.max), rec, depth);

    return hit_left || hit_right;
}

aabb bvh_node::bounding_box() const
{
    return m_bbox;
}

//aabb bvh_node::set_bounding_box(const aabb& bbox2) const
//{
//    bbox = aabb(bbox2.x, bbox2.y, bbox2.z);
//}

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