#include "Camera.h"

Camera::Camera(glm::mat4 camera_to_world)
{
	this->camera_to_world = camera_to_world;
	this->world_to_camera = glm::inverse(camera_to_world);
}

Camera::Camera(glm::vec3 position, glm::mat4 rotation)
{
	_position = position;
	_rotation = rotation;
}
