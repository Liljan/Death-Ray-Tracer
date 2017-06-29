#include <iostream>
#include <glm.hpp>
#include <gtx\transform.hpp>

#include "Image.h"
#include "Implicit.h"
#include "Camera.h"
#include "World.h"


namespace Settings {
	const int IMG_WIDTH = 1920;
	const int IMG_HEIGHT = 1080;

	const int RAYS_PER_PIXEL = 1;

	const float FOV = glm::radians(45.0f);
}

namespace Color {
	glm::vec3 RED(1.f, 0.f, 0.f);
}

void ray_trace(World* world, Camera* camera)
{
	float aspect_ratio = Settings::IMG_WIDTH / (float)Settings::IMG_HEIGHT;
	float scale = glm::tan(Settings::FOV * 0.5f);

	glm::vec3 origin = glm::vec4(0, 0.f, 0.f, 1.f) * camera->get_camera_to_world();

	const float width = Settings::IMG_WIDTH;
	const float height = Settings::IMG_HEIGHT;

	for (size_t h = 0; h < height; h++)
	{
		for (size_t w = 0; w < width; w++)
		{
			float x = (2.f * (w + 0.5f) / width - 1.f) * aspect_ratio * scale;
			float y = (1.f - 2.f * (h + 0.5f) / height) * scale;

			glm::vec3 dir = glm::vec4(x, y, -1.f, 1.f) * camera->get_camera_to_world();
			dir = glm::normalize(dir);

			Ray* r = new Ray(origin, dir, 1.0f);
		}
	}
}

int main()
{
	Ray* test_ray = new Ray(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 1.0f), 1.0f);

	Material* m = new Material;
	m->color = Color::RED;

	glm::mat4 position = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 0.f, 5.f));

	//Sphere test_sphere(position, m, 1.0f);
	Sphere* sphere = new Sphere(position, m, 1.0f);

	Intersection* intersection = sphere->intersection(test_ray);

	/*Image img(Settings::IMG_WIDTH, Settings::IMG_HEIGHT);
	img.fill_image(Color::RED);
	img.save_PPM("image_00");*/
	return 0;
}