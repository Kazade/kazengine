/**
TODO:
	1. Convert the data to some common internal structure
	2. Sort out endianness!
*/


#include <iostream>

#include "map/basic_face.h"
#include "quake3_bsp_map.h"

file_load_status quake3_bsp_map::load(istream& stream) {

	if (!stream.good()) {
		m_last_error = "The stream was invalid";
		return FILE_NOT_FOUND;
	}

	if (!load_and_check_header(stream)) {
		m_last_error = "Map contains an invalid header string";
		return FILE_LOAD_FAILED;
	}

	read_lumps(stream);
	read_entities(stream);
	read_vertices(stream);
	read_faces(stream);
	read_face_indices(stream);
	read_textures(stream);
	read_lightmaps(stream);
	read_bsp_nodes(stream);
	read_bsp_leaves(stream);
	read_bsp_leaf_faces(stream);
	read_bsp_leaf_brushes(stream);
	read_models(stream);
	read_planes(stream);
	read_brushes(stream);
	read_brushsides(stream);
	read_effects(stream);
	read_lightvols(stream);

	do_post_load();

	print_statistics();

	return FILE_LOAD_SUCCESS;
}

void quake3_bsp_map::unload() {

}


bool quake3_bsp_map::load_and_check_header(istream& stream) {
	/*
		We load the header from the stream and check that it contains a valid ID
	*/

	stream.read(reinterpret_cast<char*>(&m_header), sizeof(quake3_header));

	char test[] = { 'I', 'B', 'S', 'P' }; //This is what it should be

	for (short i = 0; i < 4; ++i) { //Check each element
		if (test[i] != m_header.bsp_id[i])
			return false; //Return false if any of them do not match
	}

	return true;
}

void quake3_bsp_map::read_lumps(istream& stream) {
	#ifdef __DEBUG__
		std::cout << "Loading lumps" << std::endl;
	#endif
	m_lumps.resize(iMaxLumpsOffset); //Resize the lumps array
	stream.read(reinterpret_cast<char*>(&m_lumps[0]), sizeof(quake3_file_lump) * iMaxLumpsOffset);
}

void quake3_bsp_map::read_entities(istream& stream) {
	#ifdef __DEBUG__
		std::cout << "Reading entities" << std::endl;
	#endif
	int entity_length = m_lumps[iEntitiesOffset].length;
	read_chunk_into_vector<char>(stream, m_raw_entity_data, entity_length, iEntitiesOffset);
}

void quake3_bsp_map::read_vertices(istream& stream) {
	#ifdef __DEBUG__
		std::cout << "Reading vertices" << std::endl;
	#endif
	int vertex_length = m_lumps[iVerticesOffset].length / sizeof(quake3_vertex);
	read_chunk_into_vector<quake3_vertex>(stream, m_raw_vertex_data, vertex_length, iVerticesOffset);
}

void quake3_bsp_map::read_face_indices(istream& stream) {
	#ifdef __DEBUG__
		std::cout << "Reading face indices" << std::endl;
	#endif
	int face_index_length = m_lumps[iMeshVerticesOffset].length / sizeof(unsigned int);
	read_chunk_into_vector<unsigned int>(stream, m_raw_face_index_data, face_index_length, iMeshVerticesOffset);
}

void quake3_bsp_map::read_faces(istream& stream) {
	#ifdef __DEBUG__
		std::cout << "Reading faces" << std::endl;
	#endif
	int face_length = m_lumps[iFacesOffset].length / sizeof(quake3_face);
	read_chunk_into_vector<quake3_face>(stream, m_raw_face_data, face_length, iFacesOffset);
}

void quake3_bsp_map::read_textures(istream& stream) {
	#ifdef __DEBUG__
		std::cout << "Reading textures" << std::endl;
	#endif
	int texture_length = m_lumps[iTextureOffset].length / sizeof(quake3_texture);
	read_chunk_into_vector<quake3_texture>(stream, m_raw_texture_data, texture_length, iTextureOffset);
}

void quake3_bsp_map::read_lightmaps(istream& stream) {
	#ifdef __DEBUG__
		std::cout << "Reading lightmaps" << std::endl;
	#endif
	int lightmap_length = m_lumps[iLightMapsOffset].length / sizeof(quake3_lightmap);
	read_chunk_into_vector<quake3_lightmap>(stream, m_raw_lightmap_data, lightmap_length, iLightMapsOffset);
}

void quake3_bsp_map::read_bsp_nodes(istream& stream) {
	#ifdef __DEBUG__
		std::cout << "Reading BSP nodes" << std::endl;
	#endif
	int bsp_node_length = m_lumps[iNodesOffset].length / sizeof(quake3_bsp_node);
	read_chunk_into_vector<quake3_bsp_node>(stream, m_raw_bsp_node_data, bsp_node_length, iNodesOffset);
}

void quake3_bsp_map::read_bsp_leaf_faces(istream& stream) {
	#ifdef __DEBUG__
		std::cout << "Reading BSP leaf faces" << std::endl;
	#endif
	int bsp_leaf_face_length = m_lumps[iLeafFacesOffset].length / sizeof(int);
	read_chunk_into_vector<int>(stream, m_raw_bsp_leaf_face_data, bsp_leaf_face_length, iLeafFacesOffset);
}

void quake3_bsp_map::read_bsp_leaves(istream& stream) {
	#ifdef __DEBUG__
		std::cout << "Reading BSP leaves" << std::endl;
	#endif
	int bsp_leaf_length = m_lumps[iLeafOffset].length / sizeof(quake3_bsp_leaf);
	read_chunk_into_vector<quake3_bsp_leaf>(stream, m_raw_bsp_leaf_data, bsp_leaf_length, iLeafOffset);
}

void quake3_bsp_map::read_bsp_leaf_brushes(istream& stream) {
	#ifdef __DEBUG__
		std::cout << "Reading BSP leaf brushes" << std::endl;
	#endif
	int bsp_leaf_brush_length = m_lumps[iLeafBrushesOffset].length / sizeof(int);
	read_chunk_into_vector<int>(stream, m_raw_bsp_leaf_brush_data, bsp_leaf_brush_length, iLeafBrushesOffset);
}

void quake3_bsp_map::read_planes(istream& stream) {
	#ifdef __DEBUG__
		std::cout << "Reading planes" << std::endl;
	#endif
	int plane_length = m_lumps[iPlanesOffset].length / sizeof(quake3_plane);
	read_chunk_into_vector<quake3_plane>(stream, m_raw_plane_data, plane_length, iPlanesOffset);
}

void quake3_bsp_map::read_models(istream& stream) {
	#ifdef __DEBUG__
		std::cout << "Reading models" << std::endl;
	#endif
	int model_length = m_lumps[iModelsOffset].length / sizeof(quake3_map_model);
	read_chunk_into_vector<quake3_map_model>(stream, m_raw_model_data, model_length, iModelsOffset);
}

void quake3_bsp_map::read_brushes(istream& stream) {
	#ifdef __DEBUG__
		std::cout << "Reading brushes" << std::endl;
	#endif
	int brush_length = m_lumps[iBrushesOffset].length / sizeof(quake3_brush);
	read_chunk_into_vector<quake3_brush>(stream, m_raw_brush_data, brush_length, iBrushesOffset);
}

void quake3_bsp_map::read_brushsides(istream& stream) {
	#ifdef __DEBUG__
		std::cout << "Reading brushsides" << std::endl;
	#endif
	int brushside_length = m_lumps[iBrushesSidesOffset].length / sizeof(quake3_brushside);
	read_chunk_into_vector<quake3_brushside>(stream, m_raw_brushsides, brushside_length, iBrushesSidesOffset);
}

void quake3_bsp_map::read_effects(istream& stream) {
	#ifdef __DEBUG__
		std::cout << "Reading effects" << std::endl;
	#endif
	int effect_length = m_lumps[iShaderFilesOffset].length / sizeof(quake3_effect);
	read_chunk_into_vector<quake3_effect>(stream, m_raw_effect_data, effect_length, iShaderFilesOffset);
}

void quake3_bsp_map::read_lightvols(istream& stream) {
	#ifdef __DEBUG__
		std::cout << "Reading light vols" << std::endl;
	#endif
	int lightvol_length = m_lumps[iLightVolumeOffset].length / sizeof(quake3_lightvol);
	read_chunk_into_vector<quake3_lightvol>(stream, m_raw_lightvol_data, lightvol_length, iLightVolumeOffset);
}

void quake3_bsp_map::print_statistics() {
	std::cout << "Vertices: " << m_raw_vertex_data.size() << std::endl;
	std::cout << "Faces: " << m_raw_face_data.size() << std::endl;
	std::cout << "Face indices: " << m_raw_face_index_data.size() << std::endl;
	std::cout << "Planes: " << m_raw_plane_data.size() << std::endl;
	std::cout << "Textures: " << m_raw_texture_data.size() << std::endl;
	std::cout << "Lightmaps: " << m_raw_lightmap_data.size() << std::endl;
	std::cout << "Lightvols: " << m_raw_lightvol_data.size() << std::endl;
	std::cout << "Effects: " << m_raw_effect_data.size() << std::endl;
	std::cout << "BSP Nodes: " << m_raw_bsp_node_data.size() << std::endl;
	std::cout << "BSP Leaves: " << m_raw_bsp_leaf_data.size() << std::endl;
	std::cout << "BSP leaf faces: " << m_raw_bsp_leaf_face_data.size() << std::endl;
	std::cout << "BSP leaf brushes: " << m_raw_bsp_leaf_brush_data.size() << std::endl;
	std::cout << "Models: " << m_raw_model_data.size() << std::endl;
	std::cout << "Brushes: " << m_raw_brush_data.size() << std::endl;
	std::cout << "Brushsides: " << m_raw_brushsides.size() << std::endl;
}

string quake3_bsp_map::get_last_error() {
	string result = m_last_error;
	m_last_error = string();
	return result;
}


void quake3_bsp_map::do_post_load() {
	convert_vertices();
	convert_faces();
}

void quake3_bsp_map::convert_vertices() {
	//TODO: Convert other elements of the vertex

	typedef vector<quake3_vertex>::iterator quake3_vertex_iterator;
	float one_over_sixtyfour = 1.0f / 64.0f;

	m_vertices.resize(m_raw_vertex_data.size());

	size_t j = 0;

	for (quake3_vertex_iterator i = m_raw_vertex_data.begin();
		i != m_raw_vertex_data.end(); ++i) {

		//Copy the vertex to the global map vertex array
		memcpy(&m_vertices[j].position, &(*i).position, sizeof(float) * 3);

		//Scale the vertices down
		m_vertices[j].position.x *= one_over_sixtyfour;
		m_vertices[j].position.y *= one_over_sixtyfour;
		m_vertices[j].position.z *= one_over_sixtyfour;

		swap_axis(m_vertices[j].position); //Convert to normal coordinate system

		++j; //Move to the next vertex

		#ifdef __DEBUG__
			std::cout << ".";
		#endif
	}
	#ifdef __DEBUG__
		std::cout << std::endl;
	#endif
}


void quake3_bsp_map::add_normal_face(const quake3_face& f) {
	shared_ptr<map_face> new_face(new basic_face());
	new_face->clear();

	//Get the triangles that make up this face
	for (int i = 0; i < f.totalMeshVertices; i+=3) {
		int idx1 = f.startVertexIndex + m_raw_face_index_data[f.meshVertexIndex + i];
		int idx2 = f.startVertexIndex + m_raw_face_index_data[f.meshVertexIndex + i + 2];
		int idx3 = f.startVertexIndex + m_raw_face_index_data[f.meshVertexIndex + i + 1];

		//Add the triangle to the new face
		new_face->add_triangle(m_vertices[idx1], m_vertices[idx2], m_vertices[idx3]);
	}

	new_face->set_texture_index(f.texID); //Set the index to the texture array
	m_faces.push_back(new_face);
}

void quake3_bsp_map::convert_faces() {
	typedef vector<quake3_face>::iterator quake3_face_iterator;

	//Reserve enough for 3 * num faces. it may be more or less than
	//that but it doesnt matter, we are just preventing loads
	// of mem copying
	m_faces.reserve(m_raw_face_data.size() * 3);

	for (quake3_face_iterator face = m_raw_face_data.begin();
		face != m_raw_face_data.end(); ++face) {

		switch((*face).type) {
			case 1:
			#ifndef NDEBUG
				std::cout << ".";
			#endif

				add_normal_face((*face));
			default:
				continue;
		}
	}

	#ifndef NDEBUG
		std::cout << std::endl;
	#endif
}
