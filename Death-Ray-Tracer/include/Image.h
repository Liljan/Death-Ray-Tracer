#pragma once

#include <glm\glm.hpp>

using namespace glm;

class Image
{
public:
	Image(const char32_t &w, const char32_t &h) : width{ w }, height{ h } { pixelValues = new float[3 * w*h]; }
	~Image() { delete[] pixelValues; }

	void setPixel(const char32_t &x, const char32_t &y, const vec3 &color);
	void fillImage(const vec3 &color); // mostly used for testing
	void saveAsPPM(const char *filename);

private:
	float* pixelValues;
	char32_t width, height;
};