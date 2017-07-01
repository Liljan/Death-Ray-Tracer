#pragma once

#include "Light.h"
#include "Implicit.h"
#include <vector>

class World {
public:
	World();
	~World();

	void add_geometry(Implicit* geometry);
	void add_light(Light* light);

	Implicit* get_geometry(size_t i) { return m_geometry[i]; }
	Light* get_light(size_t i) { return m_lights[i]; }

	size_t get_number_of_geometry() { return m_geometry.size(); }
	size_t get_number_of_lights() { return m_lights.size(); }

private:
	std::vector<Implicit*> m_geometry;
	std::vector<Light*> m_lights;
};
