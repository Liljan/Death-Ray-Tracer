#include <iostream>
// include GLM stuff
#include <glm.hpp>

#include "Image.h"

namespace Color {
	glm::vec3 RED(1.f, 0.f, 0.f);
}

int main()
{
	Image img(1920, 1080);
	img.fill_image(Color::RED);
	img.save_PPM("image_00");
	return 0;
}