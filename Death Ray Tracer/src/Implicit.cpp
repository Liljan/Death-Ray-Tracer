#include "..\include\Implicit.h"

#include "gtx\projection.hpp"

Sphere::Sphere(const glm::mat4 & world_coords, Material * mat, float radius)
{
	_local_to_world = world_coords;
	_world_to_local = glm::inverse(world_coords);

	_center = glm::vec3(world_coords * glm::vec4(0, 0, 0, 1));

	_material = mat;
	_radius = radius;
}

Sphere::~Sphere()
{
}

Intersection * Sphere::intersection(Ray * ray)
{
	glm::vec3 v = glm::normalize(_center - ray->direction);

	float tca = glm::dot(v, ray->direction);

	if (tca < 0.0f)
	{
		return nullptr;
	}

	float d2 = glm::dot(v, v) - tca * tca;

	// case 1: no intersection
	if (d2 > _radius * _radius)
	{
		return nullptr;
	}

	float thc = sqrt(_radius*_radius - d2);

	float t_front = tca - thc;
	float t_back = tca + thc;

	Intersection* intersection = new Intersection(i1, i2, glm::normalize(i1 - _center), _material->color);
}
