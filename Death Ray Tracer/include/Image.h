#pragma once
#include <glm.hpp>

class Image
{
public:
	Image(const size_t w, const size_t h);
	~Image();

	// public methods
	void set_pixel(size_t x, size_t y, glm::vec3 & color);
	void fill_image(glm::vec3 & color); // mostly used for debugging
	void save_PPM(const char* file_name);

private:

	float* m_pixel_values;
	size_t m_width, m_height;
};