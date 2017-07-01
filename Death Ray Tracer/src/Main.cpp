#include <iostream>
#include <glm.hpp>
#include <gtx\transform.hpp>

#include "Image.h"
#include "Implicit.h"
#include "Camera.h"
#include "World.h"
#include "Light.h"

namespace Settings {
	const int IMG_WIDTH = 4;
	const int IMG_HEIGHT = 4;

	const int RAYS_PER_PIXEL = 1;

	const float FOV = glm::radians(45.0f);
}

namespace Color {
	glm::vec3 RED(1.f, 0.f, 0.f);
	glm::vec3 BLUE(0.0f, 0.0f, 1.0f);
	glm::vec3 WHITE(1.0f, 1.0f, 1.0f);
}

Image* ray_trace(World* world, Camera* camera)
{
	Image* image = new Image(Settings::IMG_WIDTH, Settings::IMG_HEIGHT);

	float aspect_ratio = Settings::IMG_WIDTH / (float)Settings::IMG_HEIGHT;
	float scale = glm::tan(Settings::FOV * 0.5f);

	glm::vec3 origin = glm::vec4(0, 0.f, 0.f, 1.f) * camera->get_camera_to_world();

	const float width = Settings::IMG_WIDTH;
	const float height = Settings::IMG_HEIGHT;

	for (size_t h = 0; h < height; h++)
	{
		for (size_t w = 0; w < width; w++)
		{
			float x = (2.0f * (w + 0.5f) / width - 1.0f) * aspect_ratio * scale;
			float y = (1.0f - 2.0f * (h + 0.5f) / height) * scale;

			glm::vec3 dir = glm::vec4(x, y, 1.0f, 1.0f) * camera->get_camera_to_world();
			dir = glm::normalize(dir);

			Ray r(origin, dir, 1.0f);

			// test intersection with all objects
			for (size_t i = 0; i < world->get_number_of_geometry(); i++)
			{
				Implicit* geometry = world->get_geometry(i);
				Intersection* intersection = geometry->intersection(&r);
				if (intersection == nullptr)
					image->set_pixel(w, h, glm::vec3(0.f, 0.f, 0.f));
				else
					image->set_pixel(w, h, intersection->color);

				delete intersection;
			}
		}
	}

	return image;
}

int main()
{
	// Detect memery leaks.
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// Camera(s)
	Camera* camera = new Camera(glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 0.f, 0.f)));

	// Light(s)
	World* world = new World();
	world->add_light(new Light(glm::vec3(10.0f, 1.0f, 1.0f), Color::WHITE, 1.0f));

	// Materials
	Material red_mat, blue_mat, white_mat;
	red_mat.color = Color::RED;
	blue_mat.color = Color::BLUE;
	white_mat.color = Color::WHITE;

	// Geometry
	Sphere sphere(glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 0.f, 5.f)), &blue_mat, 1.0f);

	world->add_geometry(&sphere);

	Image* image = ray_trace(world, camera);
	image->save_PPM("test_render");

	// Clean up
	delete camera;
	delete world;
	delete image;

	return 0;
}