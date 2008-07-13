#ifndef QUAKE3_BSP_MAP_H_INCLUDED
#define QUAKE3_BSP_MAP_H_INCLUDED

#include <map>

#include "map/bsp_map.h"

using std::map;

struct quake3_point_i{
	int x, y, z;
};

struct quake3_point_3f {
	float x, y, z;
};

struct quake3_point_2f {
	float x, y;
};

struct quake3_header {
	 char bsp_id[4];
	 int version;
};

enum quake3_file_offsets {
	 iEntitiesOffset = 0,
	 iTextureOffset,
	 iPlanesOffset,
	 iNodesOffset,
	 iLeafOffset,
	 iLeafFacesOffset,
	 iLeafBrushesOffset,
	 iModelsOffset,
	 iBrushesOffset,
	 iBrushesSidesOffset,
	 iVerticesOffset,
	 iMeshVerticesOffset,
	 iShaderFilesOffset,
	 iFacesOffset,
	 iLightMapsOffset,
	 iLightVolumeOffset,
	 iVisibleDataOffset,
	 iMaxLumpsOffset
};

struct quake3_file_lump {
	 int offset;
	 int length;
};

struct quake3_entity {
	map<string, string> attributes;
};

struct quake3_vertex {
	quake3_point_3f position;	//! Position of vertex
	quake3_point_2f texCoord;	//! (u,v) Texturecoordinate of detailtexture
	quake3_point_2f lightmap;	//! (u,v) Texturecoordinate of lightmap
	quake3_point_3f normal;	//! vertex normale
	unsigned char color[4];		//! Color in RGBA
};

//! \struct A face in bsp format info
struct quake3_face {
	 int texID;
	 int effect;
	 int type;
	 int startVertexIndex;
	 int totalVertices;
	 int meshVertexIndex;
	 int totalMeshVertices;
	 int lightmapID;
	 int lightMapCorner[2];
	 int lightMapSize[2];
	 quake3_point_3f lightMapPos;
	 quake3_point_3f lightMapVectors[2];
	 quake3_point_3f normal;
	 int size[2];
};

struct quake3_texture {
	 char file[64];
	 int flags;
	 int contents;
};

struct quake3_lightmap {
	 unsigned char lightMap[128][128][3];
};

struct quake3_bsp_node {
 int planeIndex;
 int frontIndex;
 int backIndex;
 quake3_point_i aabbMin;
 quake3_point_i aabbMax;
};

struct quake3_bsp_leaf {
 int cluster;
 int portal;
 quake3_point_i aabbMin;
 quake3_point_i aabbMax;
 int faceIndex;
 int totalFaces;
 int brushIndex;
 int totalBrushes;
};

struct quake3_brush {
	int brushside; // 	First brushside for brush.
	int brushside_count; // 	Number of brushsides for brush.
	int texture; // 	Texture index.
};

struct quake3_plane {
	float normal[3];
	float dist;
};

struct quake3_map_model {
	float mins[3]; // 	Bounding box min coord.
	float maxs[3]; // 	Bounding box max coord.
	int face; // 	First face for model.
	int face_count; // 	Number of faces for model.
	int brush; // 	First brush for model.
	int brush_count; // 	Number of brushes for model.
};

struct quake3_brushside {
	int plane; // 	Plane index.
	int texture; // 	Texture index.
};

struct quake3_effect {
	char name[64]; // 	Effect shader.
	int brush; // 	Brush that generated this effect.
	int unknown; // 	Always 5, except in q3dm8, which has one effect with -1.
};

struct quake3_lightvol {
		unsigned char ambient[3];
		unsigned char directional[3];
		unsigned char dir[2];
};


class quake3_subpatch {
	public:
		quake3_subpatch();

		vector<map_vertex>& getVertices() { return m_vertices; }

		map_vertex* get_control_points() { return m_control_points; }

		void set_l(int l) { L = l; }
		int get_l() { return L; }

		void tesselate_vertices();
		void calculate_indices();

		void append_triangles_to_array(vector<map_vertex>& vertices);
	private:
		vector<map_vertex> m_vertices;
		vector<unsigned int> m_indices;
		map_vertex 	m_control_points[9];
		int L;
};

/**
Loads a Quake3 map
*/

class quake3_bsp_map : public bsp_map {
	public:
		quake3_bsp_map(resource_manager_interface* owning_manager);

		file_load_status load(istream& stream);
		void unload();
		string get_last_error();

	private:
		bool load_and_check_header(istream& stream);
		void read_lumps(istream& stream);
		void read_entities(istream& stream);
		void read_vertices(istream& stream);
		void read_faces(istream& stream);
		void read_face_indices(istream& stream);
		void read_textures(istream& stream);
		void read_lightmaps(istream& stream);
		void read_bsp_nodes(istream& stream);
		void read_bsp_leaves(istream& stream);
		void read_bsp_leaf_faces(istream& stream);
		void read_bsp_leaf_brushes(istream& stream);
		void read_planes(istream& stream);
		void read_models(istream& stream);
		void read_brushes(istream& stream);
		void read_brushsides(istream& stream);
		void read_effects(istream& stream);
		void read_lightvols(istream& stream);

		quake3_header m_header;
		vector<quake3_file_lump> m_lumps;
		vector<quake3_entity> m_entities;

		vector<char> m_raw_entity_data;
		vector<quake3_vertex> m_raw_vertex_data;
		vector<quake3_face> m_raw_face_data;
		vector<unsigned int> m_raw_face_index_data;
		vector<quake3_texture> m_raw_texture_data;
		vector<quake3_lightmap> m_raw_lightmap_data;
		vector<quake3_bsp_node> m_raw_bsp_node_data;
		vector<quake3_bsp_leaf> m_raw_bsp_leaf_data;
		vector<int> m_raw_bsp_leaf_face_data;
		vector<int> m_raw_bsp_leaf_brush_data;
		vector<quake3_plane> m_raw_plane_data;
		vector<quake3_map_model> m_raw_model_data;
		vector<quake3_brush> m_raw_brush_data;
		vector<quake3_brushside> m_raw_brushsides;
		vector<quake3_effect> m_raw_effect_data;
		vector<quake3_lightvol> m_raw_lightvol_data;

		string m_last_error;

		template <typename T>
		bool read_chunk_into_vector(istream& stream, vector<T>& vec, size_t count, quake3_file_offsets offset);

		template <typename T>
		void swap_axis(T& v);

		void print_statistics();


		void do_post_load();
		void convert_vertices();
		void convert_faces();
		void convert_and_load_textures();

		void add_normal_face(const quake3_face& f);
		void add_curved_surface(const quake3_face& f);
};

template <typename T>
bool quake3_bsp_map::read_chunk_into_vector(istream& stream, vector<T>& vec, size_t count, quake3_file_offsets offset) {
	stream.seekg(m_lumps[offset].offset, std::ios_base::beg);
	vec.resize(count);
	stream.read(reinterpret_cast<char*>(&vec[0]), count * sizeof(T));
	return true;
}

template <typename T>
void quake3_bsp_map::swap_axis(T& v) {
    float temp = v.y;
    v.y = v.z;
    v.z = -temp;
}

#endif // QUAKE3_BSP_MAP_H_INCLUDED
