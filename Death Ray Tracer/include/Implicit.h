#pragma once

#include "glm.hpp"
#include "Ray.h"
#include "Intersection.h"

struct Material {
	glm::vec3 color;
};

class Implicit {
public:
	virtual ~Implicit() {};
	virtual Intersection* intersection(Ray* ray) = 0;
protected:

	Material* _material;
	glm::mat4x4 _local_to_world;
	glm::mat3x4 _world_to_local;

	glm::vec3 _center;
};

class Sphere : public Implicit {
public:
	Sphere(const glm::mat4 & world_coords, Material* mat, float radius);
	~Sphere();

	Intersection* intersection(Ray* ray);
private:
	float _radius;
	float _radius_squared;
};