#include "map_vertex.h"

map_vertex::map_vertex() {
	kmVec3Zero(&position);
	kmVec3Zero(&normal);
}

map_vertex map_vertex::operator+(const map_vertex& rhs) const {
	map_vertex result;
	result.position = position + rhs.position;
	result.texture_coordinate = texture_coordinate + rhs.texture_coordinate;
	result.lightmap_coordinate = lightmap_coordinate + rhs.lightmap_coordinate;
	result.normal = normal + rhs.normal;

	//FIXME: Blend the colour?

	kmVec3Normalize(&result.normal, &result.normal);

	return result;
}

map_vertex map_vertex::operator*(const float& rhs) const {
	map_vertex result;
	result.position = position * rhs;
	result.texture_coordinate = texture_coordinate * rhs;
	result.lightmap_coordinate = lightmap_coordinate * rhs;
	result.normal = normal * rhs;

	//FIXME: Blend the colour?

	kmVec3Normalize(&result.normal, &result.normal);

	return result;
}
