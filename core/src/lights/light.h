#ifndef LIGHT_H
#define LIGHT_H

#include "../primitives/hittable.h"


/// <summary>
/// Abstract class for lights
/// </summary>
class light : public hittable
{
public:
    virtual ~light() = default;


private:
    /// <summary>
    /// Update the internal AABB of the mesh.
    /// Warning: run this when the mesh is updated.
    /// </summary>
    void updateBoundingBox() override
    {
        // to implement
    }


protected:
    aabb bbox; // bounding box
    std::shared_ptr<material> mat;
    double intensity;
    color c;
};


#endif