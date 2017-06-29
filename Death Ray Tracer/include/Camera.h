#pragma once

#include "glm.hpp"

class Camera {
public:
	Camera(glm::mat4 camera_to_world);
	Camera() {};
	
	glm::mat4 get_camera_to_world() { return camera_to_world; }

private:
	glm::mat4 camera_to_world;
	glm::mat4 world_to_camera;
};