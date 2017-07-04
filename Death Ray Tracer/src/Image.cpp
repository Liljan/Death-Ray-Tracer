#include "Image.h"

#include <omp.h>

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

Image::Image(const size_t w, const size_t h)
{
	m_width = w;
	m_height = h;

	m_pixel_values = new float[3 * m_width * m_height];
}

Image::~Image()
{
	delete[] m_pixel_values;
}

void Image::set_pixel(size_t x, size_t y, glm::vec3 & color)
{
	m_pixel_values[(x + y * m_width) * 3] = color.r;
	m_pixel_values[(x + y * m_width) * 3 + 1] = color.g;
	m_pixel_values[(x + y * m_width) * 3 + 2] = color.b;
}

void Image::fill_image(glm::vec3 & color)
{
	#pragma omp parallel
	#pragma omp for
	for (int y = 0; y < m_height; y++)
	{
		for (int x = 0; x < m_width; x++)
		{
			set_pixel(x, y, color);
		}
	}
}

void Image::save_PPM(const char* file_name)
{
	std::cout << "Beginning image saving..." << std::endl;
	if (m_width == 0 || m_height == 0) { fprintf(stderr, "Can't save an empty image\n"); return; }

	std::ofstream out_file;

	std::string file_name_str = file_name;
	file_name_str.append(".ppm");


	try {
		out_file.open(file_name_str.c_str(), std::ios::binary);					// Need to spec. binary mode for Windows
		if (out_file.fail()) throw("Can't open output file");
		out_file << "P6\n" << m_width << " " << m_height << "\n255\n";	// Specific header for PPM images

		float r, g, b;
		float* current_pixel = m_pixel_values;

		// loop over each pixel in the image, clamp from 0 to 255, convert to char format and write to file. 
		for (int h = 0; h < m_height; ++h) {
			for (int w = 0; w < m_width; ++w) {

				r = std::max(0.0f, roundf(std::min(1.f, current_pixel[0]) * 255));
				g = std::max(0.0f, roundf(std::min(1.f, current_pixel[1]) * 255));
				b = std::max(0.0f, roundf(std::min(1.f, current_pixel[2]) * 255));

				out_file << static_cast<char>(r) << static_cast<char>(g) << static_cast<char>(b);
				current_pixel += 3;
			}
		}
		out_file.close();
		std::cout << "Successfully saved image!" << std::endl;
	}
	catch (const char *err) {
		fprintf(stderr, "%s\n", err);
		out_file.close();
	}
}
