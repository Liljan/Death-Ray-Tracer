#pragma once

#include "glm.hpp"
#include "Material.h"

class Intersection {

public:

	Intersection(float t, glm::vec3 front, glm::vec3 back, glm::vec3 normal, Material* mat)
	{
		distance_from_camera = t;
		intersection_front = front;
		intersection_back = back;
		surface_normal = normal;

		material = mat;
	};

	Intersection() {};

	float distance_from_camera;
	glm::vec3 intersection_front;
	glm::vec3 intersection_back;
	glm::vec3 surface_normal;

	Material* material;
};

