#ifndef MAP_VERTEX_H
#define MAP_VERTEX_H

#include <kazmathxx/vec2.h>
#include <kazmathxx/vec3.h>
#include "colour4.h"

struct map_vertex {
		Vec3 position;
		Vec2 texture_coordinate;
		Vec2 lightmap_coordinate;
		Vec3 normal;
		Colour4 colour;
};

#endif // MAP_VERTEX_H
