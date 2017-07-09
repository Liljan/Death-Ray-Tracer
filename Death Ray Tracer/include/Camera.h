#pragma once

#include "glm.hpp"

class Camera {
public:
	Camera(glm::vec3 position, glm::mat4 rotation);
	Camera() {};

	glm::vec3 get_position() { return _position; }
	glm::mat4 get_rotation() { return _rotation; }

private:
	glm::mat4 _rotation;
	glm::vec3 _position;
};