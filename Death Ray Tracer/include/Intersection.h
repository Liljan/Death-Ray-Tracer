#pragma once

#include "glm.hpp"

struct Intersection {
	glm::vec3 intersection_point;
	glm::vec3 surface_normal;
	glm::vec3 color;
};