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

	normal = glm::normalize((p2 - p1) * (p1 - p0));

	_material = mat;
}

Plane::~Plane()
{
}

Intersection * Plane::intersection(Ray * ray)
{
	glm::vec3 ray_pos_local = _world_to_local * glm::vec4(ray->origin, 1.0f);
	glm::vec3 ray_dir_local = _world_to_local *  glm::vec4(ray->direction, 1.0f);

	return nullptr;
}
