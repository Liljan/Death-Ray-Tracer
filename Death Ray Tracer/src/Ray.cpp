#include "Ray.h"

Ray::Ray(glm::vec3 position, glm::vec3 direction, float intensity)
{
	this->position = position;
	this->direction = direction;
	this->intensity = intensity;
}
