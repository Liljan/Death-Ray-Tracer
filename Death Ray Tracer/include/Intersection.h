#pragma once

#include "glm.hpp"
#include "Material.h"

struct Intersection {
	float distance;
	glm::vec3 front;
	glm::vec3 back;
	glm::vec3 normal;
	Material* material;
};
