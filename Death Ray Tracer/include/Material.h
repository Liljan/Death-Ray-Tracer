#pragma once

#include "glm.hpp"

struct Material {
	glm::vec3 color;
	float ambient;
	float diffuse;
	float specular;
	float shininess;
};
