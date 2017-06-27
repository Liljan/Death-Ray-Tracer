#include "World.h"

World::World()
{
}

World::~World()
{
	auto geometryIterator = m_geometry.begin();
	while (geometryIterator != m_geometry.end()) {
		delete *geometryIterator;
		geometryIterator++;
	}

	auto lightIterator = m_lights.begin();
	while (lightIterator != m_lights.end()) {
		delete *lightIterator;
		lightIterator++;
	}
}

void World::add_geometry(Implicit * geometry)
{
	m_geometry.push_back(geometry);
}

void World::add_light(Light * light)
{
	m_lights.push_back(light);
}
