#include "Camera.h"

Camera::Camera(glm::mat4 camera_to_world)
{
	this->camera_to_world = camera_to_world;
	this->world_to_camera = glm::inverse(camera_to_world);
}
