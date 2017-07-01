#pragma once

#include "glm.hpp"

struct Light {

	Light(glm::vec3 position, glm::vec3 color, float intensity)
	{
		this->position = position;
		this->color = color;
		this->intensity = intensity;
	}

	~Light() {}

	glm::vec3 position;
	glm::vec3 color;
	float intensity;
};