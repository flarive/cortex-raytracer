#include "uvmapping.h"

#include "../constants.h"

uvmapping::uvmapping()
	: m_scale_u(1.0), m_scale_v(1.0), m_offset_u(0.0), m_offset_v(0.0)
{
}

uvmapping::uvmapping(double scale_u, double scale_v)
	: m_scale_u(scale_u), m_scale_v(scale_v), m_offset_u(0.0), m_offset_v(0.0)
{
}


uvmapping::uvmapping(double scale_u, double scale_v, double offset_u, double offset_v)
	: m_scale_u(scale_u), m_scale_v(scale_v), m_offset_u(offset_u), m_offset_v(offset_v)
{
}

double uvmapping::scale_u() const
{
	return m_scale_u;
}

double uvmapping::scale_v() const
{
	return m_scale_v;
}

double uvmapping::offset_u() const
{
	return m_offset_u;
}

double uvmapping::offset_v() const
{
	return m_offset_v;
}

void get_sphere_uv(const point3& p, double& u, double& v, const uvmapping& mapping)
{
    // p: a given point on the sphere of radius one, centered at the origin.
    // u: returned value [0,1] of angle around the Y axis from X=-1.
    // v: returned value [0,1] of angle from Y=-1 to Y=+1.
    //     <1 0 0> yields <0.50 0.50>       <-1  0  0> yields <0.00 0.50>
    //     <0 1 0> yields <0.50 1.00>       < 0 -1  0> yields <0.50 0.00>
    //     <0 0 1> yields <0.25 0.50>       < 0  0 -1> yields <0.75 0.50>

    auto theta = acos(-p.y);
    auto phi = atan2(-p.z, p.x) + M_PI;

    double s = phi / (2 * M_PI);
    double t = theta / M_PI;

	u = mapping.scale_u() * s + mapping.offset_u();
	v = mapping.scale_v() * t + mapping.offset_v();
}

void get_torus_uv(const vector3& _p, vector3& _c, double& _u, double& _v, double _majorRadius, double _minorRadius)
{
	double phi = atan2(_p.y, _p.x);
	if (phi < 0) phi += 2 * M_PI; // Ensure phi is in [0, 2*pi]

	// Calculate the distance from the center of the torus in the xy-plane
	double dxy = glm::length(vector2(_p.x, _p.y) - vector2(_c.x, _c.y)) - _majorRadius;
	// Calculate the angle around the torus
	double theta = atan2(_p.z, dxy);
	if (theta < 0) theta += 2 * M_PI; // Ensure theta is in [0, 2*pi]

	// Normalize to [0, 1]
	double s = phi / (2 * M_PI);
	double t = theta / (2 * M_PI);


	// Apply texture scaling and offset to improve texture mapping
	double u_scale = 1.0; // Adjust as needed
	double v_scale = 1.0; // Adjust as needed
	double u_offset = 0.0; // Adjust as needed
	double v_offset = 0.0; // Adjust as needed

	_u = u_scale * s + u_offset;
	_v = v_scale * t + v_offset;
}

void get_cylinder_uv(const vector3& p, double& u, double& v, double radius)
{
	auto theta = std::atan2(p[0], p[2]);
	auto phi = std::atan2(p[1], radius);
	u = 1 - (theta + M_PI) / (2 * M_PI);
	v = (phi + M_PI / 2) / M_PI;
}

void get_disk_uv(const vector3& p, double& u, double& v, double radius)
{
	auto theta = std::atan2(p[0], p[2]);
	auto phi = std::atan2(p[1], radius);
	u = 1 - (theta + M_PI) / (2 * M_PI);
	v = (phi + M_PI / 2) / M_PI;
}

void get_cone_uv(const vector3& p, double& u, double& v, double radius, double height)
{
	// Calculate the angle around the cone
	float theta = atan2(p.x, p.z);
	// Calculate the distance from the cone apex to the point
	float distance = sqrt(p.x * p.x + p.z * p.z);
	// Map the angle to the range [0, 1] for u coordinate
	u = (theta + M_PI) / (2 * M_PI);
	// Map the distance to the range [0, 1] for v coordinate
	v = p.y / (height / 16.0); // Height of cone should be provided
}

void get_xy_rect_uv(double x, double y, double& u, double& v, float x0, float x1, float y0, float y1, const uvmapping& mapping)
{
	double s = (x - x0) / (x1 - x0);
	double t = (y - y0) / (y1 - y0);

	u = mapping.scale_u() * s + mapping.offset_u();
	v = mapping.scale_v() * t + mapping.offset_v();
}

void get_xz_rect_uv(double x, double z, double& u, double& v, float x0, float x1, float z0, float z1, const uvmapping& mapping)
{
	double s = (x - x0) / (x1 - x0);
	double t = (z - z0) / (z1 - z0);

	u = mapping.scale_u() * s + mapping.offset_u();
	v = mapping.scale_v() * t + mapping.offset_v();
}

void get_yz_rect_uv(double y, double z, double& u, double& v, float y0, float y1, float z0, float z1, const uvmapping& mapping)
{
	double s = (y - y0) / (y1 - y0);
	double t = (z - z0) / (z1 - z0);

	u = mapping.scale_u() * s + mapping.offset_u();
	v = mapping.scale_v() * t + mapping.offset_v();
}