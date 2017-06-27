#pragma once

#include "glm.hpp"

class Intersection {

public:

	Intersection(glm::vec3 front, glm::vec3 back, glm::vec3 normal, glm::vec3 col)
	{
		intersection_front = front;
		intersection_back = back;
		surface_normal = normal;
		color = col;
	};

	Intersection() {};

	glm::vec3 intersection_front;
	glm::vec3 intersection_back;
	glm::vec3 surface_normal;
	glm::vec3 color;
};

