#ifndef MAP_VERTEX_H
#define MAP_VERTEX_H

#include <kazmathxx/vec2.h>
#include <kazmathxx/vec3.h>
#include "colour4.h"

struct map_vertex {
	map_vertex() {
		kmVec3Zero(&position);
		kmVec3Zero(&normal);
	}

	map_vertex(const map_vertex& rhs) {
		kmVec3Assign(&position, &rhs.position);
	}

	const map_vertex operator=(const map_vertex& rhs) {
		kmVec3Assign(&position, &rhs.position);
		return *this;
	}

		Vec3 position;
		Vec2 texture_coordinate;
		Vec2 lightmap_coordinate;
		Vec3 normal;
		Colour4 colour;
};

#endif // MAP_VERTEX_H
