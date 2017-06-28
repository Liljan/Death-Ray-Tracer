#include <iostream>
#include <glm.hpp>
#include <gtx\transform.hpp>

#include "Image.h"
#include "Implicit.h"

namespace Settings {
	const int IMG_WIDTH = 1920;
	const int IMG_HEIGHT = 1080;

	const int RAYS_PER_PIXEL = 1;

	const float FOV = 45.0f;
}

namespace Color {
	glm::vec3 RED(1.f, 0.f, 0.f);
}

int main()
{
	Ray* test_ray = new Ray(glm::vec3(0, 0, 0), glm::vec3(0, 0, 1.0f), 1.0f);

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