#pragma once

#include "glm.hpp"

class Ray {
public:

	Ray(glm::vec3 position, glm::vec3 direction, float intensity);
	~Ray() {};

	glm::vec3 position;
	glm::vec3 direction;
	float intensity;
};
