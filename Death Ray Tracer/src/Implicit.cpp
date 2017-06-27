#include "..\include\Implicit.h"

#include "gtx\projection.hpp"

Sphere::Sphere(glm::mat4x4 world_coords, Material * mat, float radius)
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

	if (glm::dot(ray->direction, v) < 0.0f)
		return nullptr;

	glm::vec3 projection_point = glm::proj(v, ray->direction);
	glm::vec3 center_to_projection_point = _center - projection_point;

	float ctpp = glm::length(center_to_projection_point);

	// case 1: no intersection
	if (ctpp > _radius)
	{
		return nullptr;
	}

	float dist = glm::sqrt(_radius*_radius - ctpp*ctpp);
	glm::vec3 i1;
	glm::vec3 i2;

	// case 2: only one intersection
	if (ctpp == _radius)
	{
		// single intersection
		i1 = projection_point - ray->direction * dist;
		i2 = i1;
	}

	else
	{
		// double intersection
		i1 = projection_point - ray->direction * dist;
		i2 = projection_point + ray->direction * dist;;
	}

	Intersection* intersection = new Intersection(i1, i2, glm::normalize(i1 - _center), _material->color);
}
