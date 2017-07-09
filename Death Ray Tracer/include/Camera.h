#pragma once

#include "glm.hpp"

class Camera {
public:
	Camera(glm::mat4 camera_to_world);
	Camera(glm::vec3 position, glm::mat4 rotation);
	Camera() {};
	
	glm::mat4 get_camera_to_world() { return camera_to_world; }

	glm::vec3 get_position() { return _position; }
	glm::mat4 get_rotation() { return _rotation; }

private:
	glm::mat4 camera_to_world;
	glm::mat4 world_to_camera;

	glm::mat4 _rotation;
	glm::vec3 _position;
};