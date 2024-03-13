#pragma once

#include <array>
#include <filesystem>
#include <string>
#include <vector>

#include <fstream>
#include <iostream>
#include <sstream>

#include <glm/glm.hpp>
#include <glm/gtx/intersect.hpp>
#include <glm/gtc/matrix_inverse.hpp>

//#include "../utilities/AABB.h"
#include "../misc/ray.h"
#include "hittable.h"
#include "../utilities/MathUtils.h"



struct MeshVertex
{
	vec3 position;
	vec3 normal;

	MeshVertex(const vec3& position, const vec3& normal) :
		position(position),
		normal(normal)
	{
		
	}
};

struct MeshFace
{
	std::array<int, 3> vertices;
	std::array<int, 3> normals;

	MeshFace(const std::array<int, 3>& vertices, const std::array<int, 3>& normals) :
		vertices(vertices),
		normals(normals)
	{

	}
};

class Mesh : public hittable
{
public:
	Mesh() :
		m_boundingBox({ 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 }),
		m_material(nullptr)
	{
	}

	explicit Mesh(std::vector<MeshVertex> vertices, std::vector<MeshFace> faces) :
		m_vertices(std::move(vertices)),
		m_faces(std::move(faces)),
		m_boundingBox({ 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 }),
		m_material(nullptr)
	{
		updateBoundingBox();
	}

	// ?????????????
	aabb bounding_box() const override
	{
		// whyyyyyyyyyyyyyyyyyy ?
		updateBoundingBox();
		
		return m_boundingBox;
	}

	// ??????????????
	bool hit(const ray& r, interval ray_t, hit_record& rec) const override
	{

		return false;
	}

	void setVertices(std::vector<MeshVertex> vertices)
	{
		m_vertices = std::move(vertices);

		updateBoundingBox();
	}

	void setFaces(std::vector<MeshFace> faces)
	{
		m_faces = std::move(faces);
	}

	void setMaterial(std::shared_ptr<material> mat)
	{
		m_material = std::move(mat);
	}

	void applyTransformation(const Mat4& transformation)
	{
		const auto normalTransformation = glm::inverseTranspose(transformation);

		// Transform all vertices and normals
		for (unsigned int i = 0; i < m_vertices.size(); i++)
		{
			vec3 pos = m_vertices[i].position;
			Vec3 posFixed = Vec3(pos.x(), pos.y(), pos.z());
			
			Vec3 aaa = mapPoint(transformation, posFixed);


			vec3 norm = m_vertices[i].normal;
			Vec3 normFixed = Vec3(norm.x(), norm.y(), norm.z());


			Vec3 bbb = mapVector(normalTransformation, normFixed);
			
			m_vertices[i].position = vec3(aaa.x, aaa.y, aaa.z);
			m_vertices[i].normal = vec3(bbb.x, bbb.y, bbb.z);
		}

		updateBoundingBox();
	}

	int nbVertices() const
	{
		return static_cast<int>(m_vertices.size());
	}

	int nbFaces() const
	{
		return static_cast<int>(m_faces.size());
	}

	const std::vector<MeshVertex>& vertices() const
	{
		return m_vertices;
	}


	const std::vector<MeshFace>& faces() const
	{
		return m_faces;
	}

	const std::shared_ptr<material>& materials() const
	{
		return m_material;
	}

	const aabb& boundingBox() const
	{
		return m_boundingBox;
	}


	/**
	 * \brief Return the transformed vertex v (between 0 and 2) from a face
	 * \param face The index of the face of the mesh
	 * \param v The index of the vertex in the face
	 * \return The 3D coordinates of the vertex
	 */
	vec3 vertex(int face, int v) const
	{
		assert(face >= 0 && face < nbFaces());
		assert(v >= 0 && v < 3);

		const auto index = m_faces[face].vertices[v];

		assert(index >= 0 && index < nbVertices());

		return m_vertices[index].position;
	}

	/**
	 * \brief Return the transformed normal at a vertex v (between 0 and 2) from a face
	 * \param face The index of the face of the mesh
	 * \param v The index of the vertex in the face
	 * \return The 3D coordinates of the normal
	 */
	vec3 normal(int face, int v) const
	{
		assert(face >= 0 && face < nbFaces());
		assert(v >= 0 && v < 3);

		const auto index = m_faces[face].normals[v];

		if (index >= 0 && index < nbVertices())
		{
			// Normal is defined in the mesh
			return m_vertices[index].normal;
		}
		else
		{
			// We need to manually compute the normal
			const auto v0 = vertex(face, 0);
			const auto v1 = vertex(face, 1);
			const auto v2 = vertex(face, 2);

			//return glm::normalize(glm::cross(v1 - v0, v2 - v0));

			// dirty !!!!!
			Vec3 v0fix = Vec3(v0.x(), v0.y(), v0.z());
			Vec3 v1fix = Vec3(v1.x(), v1.y(), v1.z());
			Vec3 v2fix = Vec3(v2.x(), v2.y(), v2.z());

			Vec3 mmm = glm::normalize(glm::cross(v1fix - v0fix, v2fix - v0fix));
			return vec3(mmm.x, mmm.y, mmm.z);
		}
	}

	/**
	 * \brief Return the transformed normal interpolated with barycentric coordinates from a face
	 * \param face The index of the face of the mesh
	 * \param u The first barycentric coordinate
	 * \param v The second barycentric coordinate
	 * \return The 3D coordinates of the normal
	 */
	vec3 normal(int face, double u, double v) const
	{
		const auto n0 = normal(face, 0);
		const auto n1 = normal(face, 1);
		const auto n2 = normal(face, 2);

		const vec3 normalVector = (1.0 - u - v) * n0 + u * n1 + v * n2;

		// dirty fix !!!!!!!!!!!!!!!!
		Vec3 normalVectorFixed = Vec3(normalVector.x(), normalVector.y(), normalVector.z());

		auto nnnn = glm::normalize(normalVectorFixed);
		vec3 nnnnFixed = vec3(nnnn.x, nnnn.y, nnnn.z);

		return nnnnFixed;
	}

private:
	/**
	 * \brief Update the internal AABB of the mesh.
	 *        Warning: run this when the mesh is updated.
	 */
	void updateBoundingBox()
	{
		if (!m_vertices.empty())
		{
			vec3 a = m_vertices.front().position;
			vec3 b = m_vertices.front().position;

			Vec3 aFixed = Vec3(a.x(), a.y(), a.z());
			Vec3 bFixed = Vec3(b.x(), b.y(), b.z());


			for (const auto& v : m_vertices)
			{
				Vec3 vPosFixed2 = Vec3(v.position.x(), v.position.y(), v.position.z());


				aFixed = glm::min(aFixed, vPosFixed2);
				bFixed = glm::max(bFixed, vPosFixed2);
			}

			// dirty FL !!!!!!!!!
			vec3 aFixed2 = vec3((double)aFixed.x, (double)aFixed.y, (double)aFixed.z);

			vec3 bFixed2 = vec3((double)bFixed.x, (double)bFixed.y, (double)bFixed.z);

			m_boundingBox = aabb(aFixed2, bFixed2);
		}
	}

	/**
	 * \brief Vertices and normals of the mesh
	 */
	std::vector<MeshVertex> m_vertices;
	
	/**
	 * \brief Faces of the mesh
	 */
	std::vector<MeshFace> m_faces;

	/**
	 * \brief Bounding box of the mesh for ray intersection acceleration
	 */
	aabb m_boundingBox;
	
	/**
	 * \brief Material of the mesh
	 */
	std::shared_ptr<material> m_material;
};

int indexOfNumberLetter(const std::string& str, int offset)
{
	const auto length = static_cast<int>(str.length());

	for (int i = offset; i < length; ++i)
	{
		if ((str[i] >= '0' && str[i] <= '9') || str[i] == '-' || str[i] == '.') return i;
	}

	return length;
}

int lastIndexOfNumberLetter(const std::string& str)
{
	const auto length = static_cast<int>(str.length());

	for (int i = length - 1; i >= 0; --i)
	{
		if ((str[i] >= '0' && str[i] <= '9') || str[i] == '-' || str[i] == '.') return i;
	}

	return length;
}

std::vector<std::string> split(const std::string& s, char delim)
{
	std::vector<std::string> elems;

	std::stringstream ss(s);
	std::string item;
	while (getline(ss, item, delim))
	{
		elems.push_back(item);
	}

	return elems;
}


bool loadMesh(const std::filesystem::path& filename, std::vector<MeshVertex>& vertices, std::vector<MeshFace>& faces)
{
	std::ifstream file(filename);

	if (!file.is_open())
	{
		std::cerr << "Mesh::load() - Could not open file " << filename << std::endl;
		return false;
	}

	// Store vertex and normal data while reading
	std::vector<vec3> raw_vertices;
	std::vector<vec3> raw_normals;
	std::vector<int> v_elements;
	std::vector<int> n_elements;

	std::string line;
	while (getline(file, line))
	{
		if (line.substr(0, 2) == "v ")
		{
			// Read position data
			int index1 = indexOfNumberLetter(line, 2);
			int index2 = lastIndexOfNumberLetter(line);
			auto values = split(line.substr(index1, index2 - index1 + 1), ' ');
			raw_vertices.emplace_back(stof(values[0]), stof(values[1]), stof(values[2]));
		}
		else if (line.substr(0, 3) == "vn ")
		{
			// Read normal data
			int index1 = indexOfNumberLetter(line, 2);
			int index2 = lastIndexOfNumberLetter(line);
			auto values = split(line.substr(index1, index2 - index1 + 1), ' ');
			raw_normals.emplace_back(stof(values[0]), stof(values[1]), stof(values[2]));
		}
		else if (line.substr(0, 2) == "f ")
		{
			// Read face data
			int index1 = indexOfNumberLetter(line, 2);
			int index2 = lastIndexOfNumberLetter(line);
			auto values = split(line.substr(index1, index2 - index1 + 1), ' ');
			for (int i = 0; i < static_cast<int>(values.size()) - 2; i++)
			{
				// Split up vertex indices
				auto v1 = split(values[0], '/'); // Triangle fan for ngons
				auto v2 = split(values[i + 1], '/');
				auto v3 = split(values[i + 2], '/');

				// Store position indices
				v_elements.push_back(std::stoul(v1[0]) - 1);
				v_elements.push_back(std::stoul(v2[0]) - 1);
				v_elements.push_back(std::stoul(v3[0]) - 1);

				// Check for normals
				if (v1.size() >= 3 && v1[2].length() > 0)
				{
					n_elements.push_back(std::stoul(v1[2]) - 1);
					n_elements.push_back(std::stoul(v2[2]) - 1);
					n_elements.push_back(std::stoul(v3[2]) - 1);
				}
			}
		}
	}

	file.close();

	// Resize vertices and faces vectors
	vertices.clear();
	vertices.reserve(raw_vertices.size());

	for (unsigned int i = 0; i < std::max(raw_vertices.size(), raw_normals.size()); i++)
	{
		vec3 vertex;
		vec3 normal;

		if (i < raw_vertices.size())
		{
			vertex = raw_vertices[i];
		}

		if (i < raw_normals.size())
		{
			normal = raw_normals[i];
		}

		vertices.emplace_back(vertex, normal);
	}

	faces.clear();
	faces.reserve(std::max(v_elements.size(), n_elements.size()) / 3);

	for (unsigned int i = 0; i < std::max(v_elements.size(), n_elements.size()); i += 3)
	{
		std::array<int, 3> v = { {0, 0, 0} };
		std::array<int, 3> n = { {-1, -1, -1} };

		if (i + 2 < v_elements.size())
		{
			v[0] = v_elements[i];
			v[1] = v_elements[i + 1];
			v[2] = v_elements[i + 2];
		}

		if (i + 2 < n_elements.size())
		{
			n[0] = n_elements[i];
			n[1] = n_elements[i + 1];
			n[2] = n_elements[i + 2];
		}

		faces.emplace_back(v, n);
	}

	return true;
}

bool loadMesh(const std::filesystem::path& filename, Mesh& mesh)
{
	std::vector<MeshVertex> vertices;
	std::vector<MeshFace> faces;

	const auto success = loadMesh(filename, vertices, faces);

	mesh.setVertices(vertices);
	mesh.setFaces(faces);

	return success;
}


bool rayMeshIntersection(
	const Mesh& mesh,
	const ray& ray,
	double minT,
	hit_record& hit)
{
	// First intersect ray with AABB to quickly discard non-intersecting rays
	if (!rayAABBIntersection(mesh.boundingBox(), ray))
	{
		return false;
	}

	const auto& orig = ray.origin();
	const auto& dir = ray.direction();

	bool intersectionFound = false;

	//hit.ray = ray; // ??????????????????????????????????????????????????????????????????
	hit.t = std::numeric_limits<double>::max();

	// Iterate over all triangles in the mesh
	for (int f = 0; f < mesh.nbFaces(); f++)
	{
		const auto& v0 = mesh.vertex(f, 0);
		const auto& v1 = mesh.vertex(f, 1);
		const auto& v2 = mesh.vertex(f, 2);

		// Distance between origin and hit along the ray
		double t;
		// Output barycentric coordinates of the intersection point
		Vec2 baryPosition;

		// Check if there is an intersection with this triangle
		//if (glm::intersectRayTriangle(orig, dir, v0, v1, v2, baryPosition, t) && t >= minT && t < hit.t)

		// dirty FL !!!!!!!!!!!!!!!!!!!!!!!!!
		Vec3 origFixed = Vec3(orig.x(), orig.y(), orig.z());
		Vec3 dirFixed = Vec3(dir.x(), dir.y(), dir.z());
		Vec3 v0Fixed = Vec3(v0.x(), v0.y(), v0.z());
		Vec3 v1Fixed = Vec3(v1.x(), v1.y(), v1.z());
		Vec3 v2Fixed = Vec3(v2.x(), v2.y(), v2.z());

		if (glm::intersectRayTriangle(origFixed, dirFixed, v0Fixed, v1Fixed, v2Fixed, baryPosition, t) && t >= minT && t < hit.t)
		{
			hit.t = t;

			hit.p = ray.at(hit.t);
			hit.normal = mesh.normal(f, baryPosition.x, baryPosition.y);
			//hit.front_face = glm::dot(ray.direction(), hit.normal) < 0;

			vec3 dddd = ray.direction();
			Vec3 ddddFixed = Vec3(dddd.x(), dddd.y(), dddd.z());

			vec3 nnnn = hit.normal;
			Vec3 nnnnFixed = Vec3(nnnn.x(), nnnn.y(), nnnn.z());

			glm::f64 aaa = glm::dot(ddddFixed, nnnnFixed);
			hit.front_face = aaa < 0;

			hit.mat = mesh.materials();

			intersectionFound = true;
		}
	}

	return intersectionFound;
}

bool rayMeshesIntersection(
	const std::vector<Mesh>& meshes,
	const ray& ray,
	double minT,
	hit_record& hit)
{
	bool intersectionFound = false;

	hit.t = std::numeric_limits<double>::max();

	// Iterate over all triangles in the mesh
	for (unsigned int m = 0; m < meshes.size(); m++)
	{
		hit_record meshHit;

		// Check if there is an intersection with this triangle
		if (rayMeshIntersection(meshes[m], ray, minT, meshHit) && meshHit.t < hit.t)
		{
			hit = meshHit;

			intersectionFound = true;
		}
	}

	return intersectionFound;
}



