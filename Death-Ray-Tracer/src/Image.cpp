#include "Image.h"

#include <iostream>
#include <fstream>
#include <algorithm>

void Image::setPixel(const char32_t & x, const char32_t & y, const vec3 & color)
{
	pixelValues[(x + y*width) * 3] = color.r;
	pixelValues[(x + y*width) * 3 + 1] = color.g;
	pixelValues[(x + y*width) * 3 + 2] = color.b;
}

void Image::fillImage(const vec3 &color)
{
	for (char32_t y = 0; y < height; y++) {
		for (char32_t x = 0; x < width; x++) {
			setPixel(x, y, color);
		}
	}
}

void Image::saveAsPPM(const char * filename)
{
	std::ofstream outFile;

	try {
		outFile.open(filename, std::ios::binary);					// Need to spec. binary mode for Windows
		if (outFile.fail()) throw("Can't open output file");
		outFile << "P6\n" << width << " " << height << "\n255\n";	// Specific header for PPM images
		float r, g, b;

		float* currentPixel = pixelValues;

		// loop over each pixel in the image, clamp from 0 to 255, convert to char format and write to file. 

		for (unsigned int h = 0; h < height; ++h) {
			for (unsigned int w = 0; w < width; ++w) {

				r = std::max(0.0f, roundf(std::min(1.f, currentPixel[0]) * 255));
				g = std::max(0.0f, roundf(std::min(1.f, currentPixel[1]) * 255));
				b = std::max(0.0f, roundf(std::min(1.f, currentPixel[2]) * 255));

				outFile << static_cast<char>(r) << static_cast<char>(g) << static_cast<char>(b);
				//std::cout << "Color at " << w << ":" << h << " = " << r << ":" << g  << ":" << b << std::endl;

				currentPixel += 3;
			}
		}
		outFile.close();
	}
	catch (const char *err) {
		fprintf(stderr, "%s\n", err);
		outFile.close();
	}
}