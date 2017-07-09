#pragma once

#include "glm.hpp"
#include "Ray.h"
#include "Intersection.h"

class Implicit {
public:
	virtual ~Implicit() {};
	virtual Intersection* intersection(Ray* ray) = 0;
	glm::vec3 get_center() { return _center; }

	Material* get_material() { return _material; }

protected:

	Material* _material;
	glm::mat4x4 _local_to_world;
	glm::mat4x4 _world_to_local;

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

class Plane : public Implicit {
public:
	Plane(const glm::mat4 & world_coords, Material* mat);
	Plane(const glm::vec3 p0, const glm::vec3 p1, const glm::vec3 p2, const glm::vec3 p3, Material* mat);
	~Plane();

	Intersection* intersection(Ray* ray);
private:
	glm::vec3 normal;

	glm::vec3 p0, p1, p2, p3;

	/*

	p0_________p3
	|           |
	|		    |
	|		    |
	p1_________p2

	*/
};