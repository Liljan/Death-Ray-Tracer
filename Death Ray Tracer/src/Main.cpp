#include <iostream>
#include <glm.hpp>
#include <gtx\transform.hpp>

#include <omp.h>
#include <time.h>

#include "Image.h"
#include "Implicit.h"
#include "Camera.h"
#include "World.h"
#include "Light.h"

namespace Settings {
	const int IMG_WIDTH = 1920;
	const int IMG_HEIGHT = 1280;

	const int RAYS_PER_PIXEL = 1;

	const float FOV = glm::radians(45.0f);
}

namespace Color {
	glm::vec3 RED(1.f, 0.f, 0.f);
	glm::vec3 GREEN(0.0f, 1.0f, 0.0f);
	glm::vec3 BLUE(0.0f, 0.0f, 1.0f);
	glm::vec3 WHITE(1.0f, 1.0f, 1.0f);
}

Image* ray_trace(World* world, Camera* camera)
{
	Image* image = new Image(Settings::IMG_WIDTH, Settings::IMG_HEIGHT);

	float aspect_ratio = Settings::IMG_WIDTH / (float)Settings::IMG_HEIGHT;
	float scale = glm::tan(Settings::FOV * 0.5f);

	//glm::vec3 origin = glm::vec4(0, 0.f, 0.f, 1.f) * camera->get_camera_to_world();
	// For debugging purposes
	glm::vec3 origin = glm::vec3(0.5f, 0.5f, -1.f);

	const int width = Settings::IMG_WIDTH;
	const int height = Settings::IMG_HEIGHT;

#pragma omp parallel
#pragma omp for
	for (int h = 0; h < height; h++)
	{
		for (int w = 0; w < width; w++)
		{
			float x = (2.0f * (w + 0.5f) / width - 1.0f) * aspect_ratio * scale;
			float y = (1.0f - 2.0f * (h + 0.5f) / height) * scale;

			glm::vec3 camera_position = glm::vec3(x, y, origin.z);
			glm::vec3 camera_direction = glm::vec4(x, y, 1.0f, 1.0f) * camera->get_camera_to_world();
			camera_direction = glm::normalize(camera_direction);

			Ray ray = { origin,camera_direction };

			// base color black for pixel...
			glm::vec3 final_color(0.f, 0.f, 0.f);

			float distance_from_camera = INFINITY;
			Intersection* closest_intersection = nullptr;
			size_t closest_geometry_index;

			// ------- INTERSECTION TEST  ------- //
			for (size_t i = 0; i < world->get_number_of_geometry(); i++)
			{
				Implicit* geometry = world->get_geometry(i);
				Intersection* current_intersection = geometry->intersection(&ray);

				// If no intersection...
				if (current_intersection == nullptr)
					continue;

				// If there is an intersection which is closer...
				if (current_intersection->distance < distance_from_camera)
				{
					delete closest_intersection;
					closest_intersection = current_intersection;
					distance_from_camera = current_intersection->distance;
					closest_geometry_index = i;
					continue;
				}

				// If there is an intersection which is further away...
				delete current_intersection;
			}

			if (closest_intersection == nullptr)
				continue;

			// ------- SHADOW RAY OBSTRUCTION TEST  ------- //

			bool is_obscured = false;

			for (size_t i = 0; i < world->get_number_of_geometry(); i++)
			{
				if (i == closest_geometry_index)
					continue;

				Implicit* geometry = world->get_geometry(i);

				glm::vec3 direction = glm::normalize(world->get_light(0)->position - closest_intersection->front);

				Ray shadow_ray = { closest_intersection->front, direction };

				Intersection* intersection = geometry->intersection(&shadow_ray);

				if (intersection != nullptr)
				{
					is_obscured = true;
					delete intersection;
					break;
				}
			}

			if (is_obscured)
				continue;

			// ------- LOCAL COLOR ------- //

			for (size_t l = 0; l < world->get_number_of_lights(); l++)
			{
				Light* light = world->get_light(l);

				// Phong shading model for local lighting

				glm::vec3 s_s = glm::normalize(light->position - closest_intersection->front);
				glm::vec3 r_s = glm::reflect(s_s, closest_intersection->normal);

				Material* mat = closest_intersection->material;
				float diffuse;
				float specular;

				diffuse = mat->diffuse * glm::max(glm::dot(s_s, closest_intersection->normal), 0.0f);
				specular = mat->specular * glm::pow(glm::max(glm::dot(r_s, camera_direction), 0.0f), mat->shininess);

				glm::vec3 diffuse_component = (mat->ambient + diffuse) * light->intensity * mat->color;
				glm::vec3 specular_component = glm::vec3(specular);

				image->set_pixel(w, h, diffuse_component + specular);
				final_color += diffuse_component + specular;
			}

			image->set_pixel(x, y, final_color);

			delete closest_intersection;
		}
	}

	return image;
}

int main()
{
	// Detect memery leaks.
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	omp_set_num_threads(4);

	// Time measurement
	clock_t t1, t2;
	t1 = clock();

	// Camera(s)
	Camera* camera = new Camera(glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 0.f, -1.f)));

	// Light(s)
	World* world = new World();
	//world->add_light(&Light(glm::vec3(0.0f, 8.0f, -3.0f), Color::WHITE, 1.0f));

	Light l = { glm::vec3(0.5f, 0.5f, 0.0f), Color::WHITE, 1.0f };
	world->add_light(&l);

	// Materials
	Material red_mat = { Color::RED, 0.1f,0.7f,1.0f,32.0f };
	Material green_mat = { Color::GREEN, 0.1f,0.7f,1.0f,32.0f };
	Material blue_mat = { Color::BLUE, 0.1f,0.7f,1.0f,32.0f };
	Material white_mat = { Color::WHITE, 0.1f,0.7f,1.0f,32.0f };

	// Geometry
	/*Sphere sphere_01(glm::translate(glm::mat4(1.0f), glm::vec3(1.f, 0.f, 5.f)), &blue_mat, 1.0f);
	world->add_geometry(&sphere_01);
	Sphere sphere_02(glm::translate(glm::mat4(1.0f), glm::vec3(3.f, 1.f, 10.f)), &red_mat, 1.0f);
	world->add_geometry(&sphere_02);
	Sphere sphere_03(glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 0.f, 15.f)), &white_mat, 5.0f);
	world->add_geometry(&sphere_03); */

	glm::vec3 v0 = glm::vec3(0, 0, 0);
	glm::vec3 v1 = glm::vec3(1, 0, 0);
	glm::vec3 v2 = glm::vec3(1, 0, 1);
	glm::vec3 v3 = glm::vec3(0, 0, 1);
	glm::vec3 v4 = glm::vec3(0, 1, 0);
	glm::vec3 v5 = glm::vec3(1, 1, 0);
	glm::vec3 v6 = glm::vec3(1, 1, 1);
	glm::vec3 v7 = glm::vec3(0, 1, 1);

	// Back
	world->add_geometry(new Plane(v2, v3, v7, v6, &white_mat));
	// Bottom
	world->add_geometry(new Plane(v0, v3, v2, v1, &white_mat));
	// Top
	world->add_geometry(new Plane(v4, v5, v6, v7, &white_mat));
	// Left
	world->add_geometry(new Plane(v0, v4, v7, v2, &green_mat));
	// Right
	world->add_geometry(new Plane(v1, v2, v6, v5, &red_mat));

	// Spheres
	world->add_geometry(new Sphere(glm::translate(glm::mat4(1.0f), glm::vec3(0.2f, 0.3f, 0.7f)), &blue_mat, 0.1f));

	Image* image = ray_trace(world, camera);
	image->save_PPM("test_render");

	// Clean up
	delete camera;
	delete world;
	delete image;

	t2 = clock();
	float diff = (float)t2 - (float)t1;
	diff /= CLOCKS_PER_SEC;
	std::cout << "Total time: " << diff;

	return 0;
}