#ifndef MAP_FACE_H
#define MAP_FACE_H

#include <vector>
#include <kazmathxx/vec3.h>

#include "map_vertex.h"

using std::vector;
/*
int texture 	Texture index.
int effect 	Index into lump 12 (Effects), or -1.
int type 	Face type. 1=polygon, 2=patch, 3=mesh, 4=billboard
int vertex 	Index of first vertex.
int n_vertexes 	Number of vertices.
int meshvert 	Index of first meshvert.
int n_meshverts 	Number of meshverts.
int lm_index 	Lightmap index.
int[2] lm_start 	Corner of this face's lightmap image in lightmap.
int[2] lm_size 	Size of this face's lightmap image in lightmap.
float[3] lm_origin 	World space origin of lightmap.
float[2][3] lm_vecs 	World space lightmap s and t unit vectors.
float[3] normal 	Surface normal.
int[2] size 	Patch dimensions.
*/

//FIXME: Some of these variables need to be moved to their respective subclasses (e.g. patch)

class map_face {
	public:
		map_face();
		virtual ~map_face();

		virtual void add_triangle(const map_vertex& v1, const map_vertex& v2, const map_vertex& v3) = 0;
		void set_texture_index(int texture_idx) { m_texture_index = texture_idx; }
		int get_texture_index() { return m_texture_index; }

		int get_triangle_count() { return m_triangle_count; }

		bool is_translucent() { return m_is_translucent; }

		void clear();

		vector<map_vertex> get_vertices() { return m_vertices; }

	protected:
		/*
			The vertices that make up the face. Basically an array
			of triangles. e.g. v1, v2, v3, v1, v3, v4 etc.
		*/
		vector<map_vertex> m_vertices;

		int m_texture_index; ///< Index into the map's texture array
		int m_effect_id; ///< Index into the map's effect array
		int m_face_type; ///< What type of face is this?
		int m_lightmap_index; ///< The index to the lightmap
		int m_lightmap_start[2]; ///< Corner of this face's lightmap image
		int m_lightmap_size[2];
		Vec3 m_lightmap_origin;
		Vec3 m_lightmap_vecs[2]; ///< World space lightmap s and t unit vectors
		Vec3 m_normal; ///< Surface normal
		int m_size[2]; ///< Patch dimensions

		bool m_is_translucent; ///< Does this face have any kind of transparency?
		int m_triangle_count;
};

#endif // MAP_FACE_H
