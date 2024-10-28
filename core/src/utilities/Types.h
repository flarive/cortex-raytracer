#pragma once

#include <glm/glm.hpp>

#include <tuple>
#include <unordered_map>


#include <Eigen/Eigen/Core>
#include <Eigen/Eigen/StdVector>
#include <Eigen/Eigen/Geometry>

using vector2 = glm::dvec2;
using vector3 = glm::dvec3;
using vector4 = glm::dvec4;

using matrix3 = glm::dmat3;
using matrix4 = glm::dmat4;

using point2 = glm::dvec2;
using point3 = glm::dvec3;

typedef Eigen::Matrix<double, 5, 1> Vector5d;


inline double vector_multiply_to_double(const vector3& v1, const vector3& v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

inline vector3 vector_modulo_operator(const vector3& v1, const vector3& v2)
{
	return vector3(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
}

/// <summary>
/// The square of the length of this vector
/// </summary>
/// <param name="v"></param>
/// <returns></returns>
inline double vector_length_squared(vector3 v)
{
	return v.x * v.x + v.y * v.y + v.z * v.z;
}


inline double vector_length(vector3 v)
{
	return glm::sqrt(vector_length_squared(v));
}

inline vector3 unit_vector(vector3 v)
{
	return v / vector3(vector_length(v), vector_length(v), vector_length(v));
}



// Define a hash function for glm::vec3
//namespace std {
//    template <>
//    struct hash<glm::dvec3> {
//        std::size_t operator()(const glm::dvec3& v) const {
//            std::size_t h1 = std::hash<float>()(v.x);
//            std::size_t h2 = std::hash<float>()(v.y);
//            std::size_t h3 = std::hash<float>()(v.z);
//            return h1 ^ (h2 << 1) ^ (h3 << 2);  // Combine hashes
//        }
//    };
//
//    template <typename T>
//    inline void hash_combine(std::size_t& seed, const T& val) {
//        seed ^= std::hash<T>()(val) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
//    }
//
//    template <typename... Types>
//    struct hash<std::tuple<Types...>> {
//        std::size_t operator()(const std::tuple<Types...>& t) const {
//            std::size_t seed = 0;
//            std::apply([&seed](const auto&... args) { (hash_combine(seed, args), ...); }, t);
//            return seed;
//        }
//    };
//}