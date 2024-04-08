#include "spot_light.h"

#include <glm/glm.hpp>

spot_light::spot_light(std::shared_ptr<texture>  a, vector3 dir, double cosTotalWidth, double cosFalloffStart, double intensity, bool invisible) :
    m_emit(a), m_spot_direction(unit_vector(dir)), m_intensity(intensity), m_cosTotalWidth(cosTotalWidth),
    m_cosFalloffStart(cosFalloffStart), m_invisible(invisible)
{

}

spot_light::~spot_light()
{

}

color spot_light::emitted(const ray& r_in, const hit_record& rec, double u, double v, const point3& p) const
{
    if (dot(rec.normal, r_in.direction()) < 0.0) {
        return(falloff(r_in.origin() - rec.hit_point) * m_emit->value(u, v, p) * m_intensity);
    }
    else {
        return(color(0, 0, 0));
    }
}

double spot_light::falloff(const vector3& w) const
{
    double cosTheta = glm::dot(m_spot_direction, unit_vector(w));
    if (cosTheta < m_cosTotalWidth) {
        return(0);
    }
    if (cosTheta > m_cosFalloffStart) {
        return(1);
    }
    double delta = (cosTheta - m_cosTotalWidth) / (m_cosFalloffStart - m_cosTotalWidth);
    return((delta * delta) * (delta * delta));
}