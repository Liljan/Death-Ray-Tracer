#include "..\include\Implicit.h"

#include "gtx\projection.hpp"

Sphere::Sphere(const glm::mat4 & world_coords, Material * mat, float radius)
{
	_local_to_world = world_coords;
	_world_to_local = glm::inverse(world_coords);

	_center = glm::vec3(world_coords * glm::vec4(0.f, 0.f, 0.f, 1.f));

	_material = mat;
	_radius = radius;
	_radius_squared = radius * radius;
}

Sphere::~Sphere()
{

}

Intersection * Sphere::intersection(Ray * ray)
{
	// geometric solution
	glm::vec3 L = _center - ray->origin;
	float tca = glm::dot(L, ray->direction);

	if (tca < 0)
		return nullptr;

	float distance_squared = glm::dot(L, L) - tca * tca;

	// Ray overshoots sphere
	if (distance_squared > _radius_squared)
		return nullptr;

	// Ray hits the sphere

	float thc = sqrt(_radius_squared - distance_squared);

	float t0 = tca - thc;
	float t1 = tca + thc;

	glm::vec3 front_pos = ray->origin + ray->direction * t0;
	glm::vec3 back_pos = ray->origin + ray->direction * t1;

	Intersection* intersection = new Intersection;
	*intersection = { t0, front_pos, back_pos, glm::normalize(front_pos - _center), _material };

	return intersection;
}

Plane::Plane(const glm::mat4x4 & world_coords, Material * mat)
{
	_local_to_world = world_coords;
	_world_to_local = glm::inverse(world_coords);

	_center = glm::vec3(world_coords * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

	p0 = world_coords * glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f);
	p1 = world_coords * glm::vec4(-0.5f, 0.5f, 0.0f, 1.0f);
	p2 = world_coords * glm::vec4(0.5f, 0.5f, 0.0f, 1.0f);
	p3 = world_coords * glm::vec4(0.5f, -0.5f, 0.0f, 1.0f);

	_material = mat;
}

Plane::Plane(const glm::vec3 p0, const glm::vec3 p1, const glm::vec3 p2, const glm::vec3 p3, Material * mat)
{
	this->p0 = p0;
	this->p1 = p1;
	this->p2 = p2;
	this->p3 = p3;

	_material = mat;
}

Plane::~Plane()
{
}

Intersection * Plane::intersection(Ray * ray)
{
	// Plane form: Ax + Bx + Cz + D = 0
	// a.k.a. N * p(x,y,z) + d = 0

	glm::vec3 p0p1 = p1 - p0;
	glm::vec3 p1p2 = p2 - p1;

	normal = glm::cross(p0p1, p1p2);

	// Is the ray and plane parallel?
	float n_dot_r = glm::dot(normal, ray->direction);

	if (glm::abs(n_dot_r) < 0.00001f)
		return nullptr;

	float d = -glm::dot(normal, p0);

	float t = -(glm::dot(normal, ray->origin) + d) / (n_dot_r);

	// Behind test
	if (t < 0.0f)
		return false;

	glm::vec3 p = ray->origin + t * ray->direction;

	// Intersection point (p) is found: Is p inside the plane?
	glm::vec3 p2p3 = p3 - p2;
	glm::vec3 p3p0 = p0 - p3;

	// Side 0
	glm::vec3 p0p = p - p0;
	glm::vec3 s0 = glm::cross(p0p1, p0p);
	if (glm::dot(normal, s0) < 0.0f)
		return nullptr;

	// Side 1
	glm::vec3 p1p = p - p1;
	glm::vec3 s1 = glm::cross(p1p2, p1p);
	if (glm::dot(normal, s1) < 0.0f)
		return nullptr;

	// Side 2
	glm::vec3 p2p = p - p2;
	glm::vec3 s2 = glm::cross(p2p3, p2p);
	if (glm::dot(normal, s2) < 0.0f)
		return nullptr;

	// Side 3
	glm::vec3 p3p = p - p3;
	glm::vec3 s3 = glm::cross(p3p0, p3p);
	if (glm::dot(normal, s3) < 0.0f)
		return nullptr;

	// the point is inside the plane

	Intersection* intersection = new Intersection;
	*intersection = { t, p, p, glm::normalize(normal), _material };

	return intersection;
}
