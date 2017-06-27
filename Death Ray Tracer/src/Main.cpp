#include <iostream>
#include <glm.hpp>

#include "Image.h"

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
	Image img(Settings::IMG_WIDTH, Settings::IMG_HEIGHT);
	img.fill_image(Color::RED);
	img.save_PPM("image_00");
	return 0;
}