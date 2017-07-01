#include <iostream>
#include <glm.hpp>
#include <gtx\transform.hpp>

#include "Image.h"
#include "Implicit.h"
#include "Camera.h"
#include "World.h"
#include "Light.h"

namespace Settings {
	const int IMG_WIDTH = 1920;
	const int IMG_HEIGHT = 1080;

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

			glm::vec3 camera_direction = glm::vec4(x, y, 1.0f, 1.0f) * camera->get_camera_to_world();
			camera_direction = glm::normalize(camera_direction);

			Ray r(origin, camera_direction, 1.0f);

			// base color black for pixel...
			image->set_pixel(w, h, glm::vec3(0.f, 0.f, 0.f));

			// test intersection with all objects
			for (size_t i = 0; i < world->get_number_of_geometry(); i++)
			{
				Implicit* geometry = world->get_geometry(i);
				Intersection* intersection = geometry->intersection(&r);
				if (intersection == nullptr)
					continue;

				//image->set_pixel(w, h, intersection->color);
				for (size_t l = 0; l < world->get_number_of_lights(); l++)
				{
					Light* light = world->get_light(l);

					// occlusion test with shadow rays

					// if it is visible...

					// Phong shading model for local lighting

					glm::vec3 s_s = glm::normalize(light->position - intersection->intersection_front);
					glm::vec3 r_s = glm::reflect(s_s, intersection->surface_normal);

					Material* mat = geometry->get_material();
					float diffuse;
					float specular;

					diffuse = mat->diffuse * glm::max(glm::dot(s_s, intersection->surface_normal), 0.0f);
					specular = mat->specular * glm::pow(glm::max(glm::dot(r_s,camera_direction),0.0f), mat->shininess);

					glm::vec3 diffuse_component = (mat->ambient + diffuse) * light->intensity * mat->color;
					glm::vec3 specular_component = glm::vec3(specular);

					image->set_pixel(w, h, diffuse_component + specular);
				}


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
	world->add_light(new Light(glm::vec3(0.0f, 1.0f, -1.0f), Color::WHITE, 1.0f));

	// Materials
	Material red_mat, blue_mat, white_mat;
	red_mat.color = Color::RED;
	blue_mat.color = Color::BLUE;
	white_mat.color = Color::WHITE;

	red_mat.ambient = blue_mat.ambient = white_mat.ambient = 0.1f;
	red_mat.diffuse = blue_mat.diffuse = white_mat.diffuse = 0.7f;
	red_mat.specular = blue_mat.specular = white_mat.specular = 1.0f;
	red_mat.shininess = blue_mat.shininess = white_mat.shininess = 32.0f;

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