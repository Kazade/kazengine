/**
TODO:
	1. Convert the data to some common internal structure
	2. Sort out endianness!
*/


#include <iostream>
#include <boost/thread/thread.hpp>
#include "resources/devil_texture.h"
#include "resources/iresource_manager.h"
#include "map/basic_face.h"
#include "quake3_bsp_map.h"

const int CURVE_LEVEL = 10;

quake3_bsp_map::quake3_bsp_map(resource_manager_interface* owning_manager):
        bsp_map(owning_manager) {

}

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
    read_visibility_data(stream);

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
#ifndef NDEBUG
    std::cout << "Loading lumps" << std::endl;
#endif
    m_lumps.resize(iMaxLumpsOffset); //Resize the lumps array
    stream.read(reinterpret_cast<char*>(&m_lumps[0]), sizeof(quake3_file_lump) * iMaxLumpsOffset);
}

void quake3_bsp_map::read_entities(istream& stream) {
#ifndef NDEBUG
    std::cout << "Reading entities" << std::endl;
#endif
    int entity_length = m_lumps[iEntitiesOffset].length;
    read_chunk_into_vector<char>(stream, m_raw_entity_data, entity_length, iEntitiesOffset);
}

void quake3_bsp_map::read_vertices(istream& stream) {
#ifndef NDEBUG
    std::cout << "Reading vertices" << std::endl;
#endif
    int vertex_length = m_lumps[iVerticesOffset].length / sizeof(quake3_vertex);
    read_chunk_into_vector<quake3_vertex>(stream, m_raw_vertex_data, vertex_length, iVerticesOffset);
}

void quake3_bsp_map::read_face_indices(istream& stream) {
#ifndef NDEBUG
    std::cout << "Reading face indices" << std::endl;
#endif
    int face_index_length = m_lumps[iMeshVerticesOffset].length / sizeof(unsigned int);
    read_chunk_into_vector<unsigned int>(stream, m_raw_face_index_data, face_index_length, iMeshVerticesOffset);
}

void quake3_bsp_map::read_faces(istream& stream) {
#ifndef NDEBUG
    std::cout << "Reading faces" << std::endl;
#endif
    int face_length = m_lumps[iFacesOffset].length / sizeof(quake3_face);
    read_chunk_into_vector<quake3_face>(stream, m_raw_face_data, face_length, iFacesOffset);
}

void quake3_bsp_map::read_textures(istream& stream) {
#ifndef NDEBUG
    std::cout << "Reading textures" << std::endl;
#endif
    int texture_length = m_lumps[iTextureOffset].length / sizeof(quake3_texture);
    read_chunk_into_vector<quake3_texture>(stream, m_raw_texture_data, texture_length, iTextureOffset);
}

void quake3_bsp_map::read_lightmaps(istream& stream) {
#ifndef NDEBUG
    std::cout << "Reading lightmaps" << std::endl;
#endif
    int lightmap_length = m_lumps[iLightMapsOffset].length / sizeof(quake3_lightmap);
    read_chunk_into_vector<quake3_lightmap>(stream, m_raw_lightmap_data, lightmap_length, iLightMapsOffset);
}

void quake3_bsp_map::read_bsp_nodes(istream& stream) {
#ifndef NDEBUG
    std::cout << "Reading BSP nodes" << std::endl;
#endif
    int bsp_node_length = m_lumps[iNodesOffset].length / sizeof(quake3_bsp_node);
    read_chunk_into_vector<quake3_bsp_node>(stream, m_raw_bsp_node_data, bsp_node_length, iNodesOffset);
}

void quake3_bsp_map::read_bsp_leaf_faces(istream& stream) {
#ifndef NDEBUG
    std::cout << "Reading BSP leaf faces" << std::endl;
#endif
    int bsp_leaf_face_length = m_lumps[iLeafFacesOffset].length / sizeof(int);
    read_chunk_into_vector<int>(stream, m_raw_bsp_leaf_face_data, bsp_leaf_face_length, iLeafFacesOffset);
}

void quake3_bsp_map::read_bsp_leaves(istream& stream) {
#ifndef NDEBUG
    std::cout << "Reading BSP leaves" << std::endl;
#endif
    int bsp_leaf_length = m_lumps[iLeafOffset].length / sizeof(quake3_bsp_leaf);
    read_chunk_into_vector<quake3_bsp_leaf>(stream, m_raw_bsp_leaf_data, bsp_leaf_length, iLeafOffset);
}

void quake3_bsp_map::read_bsp_leaf_brushes(istream& stream) {
#ifndef NDEBUG
    std::cout << "Reading BSP leaf brushes" << std::endl;
#endif
    int bsp_leaf_brush_length = m_lumps[iLeafBrushesOffset].length / sizeof(int);
    read_chunk_into_vector<int>(stream, m_raw_bsp_leaf_brush_data, bsp_leaf_brush_length, iLeafBrushesOffset);
}

void quake3_bsp_map::read_planes(istream& stream) {
#ifndef NDEBUG
    std::cout << "Reading planes" << std::endl;
#endif
    int plane_length = m_lumps[iPlanesOffset].length / sizeof(quake3_plane);
    read_chunk_into_vector<quake3_plane>(stream, m_raw_plane_data, plane_length, iPlanesOffset);
}

void quake3_bsp_map::read_models(istream& stream) {
#ifndef NDEBUG
    std::cout << "Reading models" << std::endl;
#endif
    int model_length = m_lumps[iModelsOffset].length / sizeof(quake3_map_model);
    read_chunk_into_vector<quake3_map_model>(stream, m_raw_model_data, model_length, iModelsOffset);
}

void quake3_bsp_map::read_brushes(istream& stream) {
#ifndef NDEBUG
    std::cout << "Reading brushes" << std::endl;
#endif
    int brush_length = m_lumps[iBrushesOffset].length / sizeof(quake3_brush);
    read_chunk_into_vector<quake3_brush>(stream, m_raw_brush_data, brush_length, iBrushesOffset);
}

void quake3_bsp_map::read_brushsides(istream& stream) {
#ifndef NDEBUG
    std::cout << "Reading brushsides" << std::endl;
#endif
    int brushside_length = m_lumps[iBrushesSidesOffset].length / sizeof(quake3_brushside);
    read_chunk_into_vector<quake3_brushside>(stream, m_raw_brushsides, brushside_length, iBrushesSidesOffset);
}

void quake3_bsp_map::read_visibility_data(istream& stream) {
#ifndef NDEBUG
    std::cout << "Reading visibility data" << std::endl;
#endif

    int visibility_length = m_lumps[iVisibleDataOffset].length;

    //Only read the visibility data if it exists
    if (visibility_length) {
        stream.seekg(m_lumps[iVisibleDataOffset].offset, std::ios_base::beg);
        stream.read(reinterpret_cast<char*>(&m_raw_cluster_data.totalClusters), sizeof(int));
        stream.read(reinterpret_cast<char*>(&m_raw_cluster_data.size), sizeof(int));

        uint num_clusters = m_raw_cluster_data.totalClusters * m_raw_cluster_data.size;

        m_raw_cluster_data.bitSet.resize(num_clusters);

        stream.read(reinterpret_cast<char*>(&m_raw_cluster_data.bitSet[0]), sizeof(unsigned char) * num_clusters);
    }
}

void quake3_bsp_map::read_effects(istream& stream) {
#ifndef NDEBUG
    std::cout << "Reading effects" << std::endl;
#endif
    int effect_length = m_lumps[iShaderFilesOffset].length / sizeof(quake3_effect);
    read_chunk_into_vector<quake3_effect>(stream, m_raw_effect_data, effect_length, iShaderFilesOffset);
}

void quake3_bsp_map::read_lightvols(istream& stream) {
#ifndef NDEBUG
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
    std::cout << "Clusters: " << m_raw_cluster_data.bitSet.size() << std::endl;
}

string quake3_bsp_map::get_last_error() {
    string result = m_last_error;
    m_last_error = string();
    return result;
}


void quake3_bsp_map::do_post_load() {
    convert_and_load_textures(); //Load these first so they can load in a parallel thread
    convert_vertices();
    convert_faces();
    convert_bsp_data();
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

        m_vertices[j].texture_coordinate.x = (*i).texCoord.x;
        m_vertices[j].texture_coordinate.y = 1.0f - (*i).texCoord.y;

        ++j; //Move to the next vertex

#ifndef NDEBUG
        std::cout << ".";
#endif
    }
#ifndef NDEBUG
    std::cout << std::endl;
#endif
}


void quake3_bsp_map::add_normal_face(const quake3_face& f) {
    shared_ptr<map_face> new_face(new basic_face());
    new_face->clear();

    //Get the triangles that make up this face
    for (int i = 0; i < f.totalMeshVertices; i += 3) {
        int idx1 = f.startVertexIndex + m_raw_face_index_data[f.meshVertexIndex + i];
        int idx2 = f.startVertexIndex + m_raw_face_index_data[f.meshVertexIndex + i + 2];
        int idx3 = f.startVertexIndex + m_raw_face_index_data[f.meshVertexIndex + i + 1];

        //Add the triangle to the new face
        new_face->add_triangle(m_vertices[idx1], m_vertices[idx2], m_vertices[idx3]);
    }

    new_face->set_texture_index(f.texID); //Set the index to the texture array
    m_faces.push_back(new_face);
}

quake3_subpatch::quake3_subpatch():
        L(CURVE_LEVEL) {

}

void quake3_subpatch::calculate_indices() {
    const int L1 = L + 1;

    assert(m_indices.empty() && "calculate_indices has been called twice");

    // Compute the indices
    for (int row = 0; row < L; ++row) {
        for (int col = 0; col < L; ++col) {
            uint idx[4];
            idx[0] = row * L1 + col;
            idx[1] = (row + 1) * L1 + col;
            idx[2] = row * L1 + col + 1;
            idx[3] = (row + 1) * L1 + col + 1;

            m_indices.push_back(idx[0]);
            m_indices.push_back(idx[1]);
            m_indices.push_back(idx[2]);

            m_indices.push_back(idx[1]);
            m_indices.push_back(idx[3]);
            m_indices.push_back(idx[2]);
        }
    }
}

void quake3_subpatch::tesselate_vertices() {
    assert(m_vertices.empty() && "Vertices array not empty!");

    // The number of vertices along a side is 1 + num edges
    const int L1 = L + 1;

    m_vertices.resize(L1 * L1); //Resize our vertex array
    // Compute the vertices
    for (int i = 0; i <= L; ++i) {
        double a = (double)i / L;
        double b = 1 - a;

        m_vertices[i] =
            m_control_points[0] * (b * b) +
            m_control_points[3] * (2 * b * a) +
            m_control_points[6] * (a * a);
    }

    for (int i = 1; i <= L; ++i) {
        double a = (double)i / L;
        double b = 1.0 - a;

        map_vertex temp[3];

        for (int j = 0; j < 3; ++j) {
            int k = 3 * j;
            temp[j] =
                m_control_points[k + 0] * (b * b) +
                m_control_points[k + 1] * (2 * b * a) +
                m_control_points[k + 2] * (a * a);
        }

        for (int j = 0; j <= L; ++j) {
            double a = (double)j / L;
            double b = 1.0 - a;

            map_vertex vertex =
                temp[0] * (b * b) +
                temp[1] * (2 * b * a) +
                temp[2] * (a * a);

            m_vertices[i * L1 + j] = vertex;
        }
    }
}

void quake3_subpatch::append_triangles_to_array(vector<map_vertex>& vertices) {
    vector <unsigned int>::const_iterator i = m_indices.begin();
    for (; i != m_indices.end(); ++i) {
        vertices.push_back(m_vertices[(*i)]);
    }
}

void quake3_bsp_map::add_curved_surface(const quake3_face& f) {
    int width = f.size[0], height = f.size[1]; //FIXME: get width and height

    int num_patches_wide = (width - 1) >> 1;
    int num_patches_high = (height - 1) >> 1;
    int total_patches = num_patches_high * num_patches_wide;

    vector<quake3_subpatch> sub_patches(total_patches);

    for (int y = 0; y < num_patches_high; ++y) {
        for (int x = 0; x < num_patches_wide; ++x) {
            int current_patch = y * num_patches_wide + x;

            for (int row = 0; row < 3; ++row) {
                for (int point = 0; point < 3; ++point) {
                    //Set the vertex for each point
                    int control_point_index = row * 3 + point;

                    int vertex_index = (y * 2 * width + x * 2) + row * width + point;

                    map_vertex* control_points = sub_patches[current_patch].get_control_points();


                    control_points[control_point_index] = m_vertices[f.startVertexIndex + vertex_index];
                    //m_SubPatches[currentPatch].m_ControlPoints[row * 3 + point].m_Normal.Normalize();
                }
            }

            //tesselate the subpatch
            sub_patches[current_patch].tesselate_vertices();
        }
    }

    shared_ptr<map_face> new_face(new basic_face());

    for (vector<quake3_subpatch>::iterator patch = sub_patches.begin();
            patch != sub_patches.end(); ++patch) {

        (*patch).calculate_indices();
        (*patch).append_triangles_to_array(new_face->get_vertices());
    }

    new_face->set_texture_index(f.texID); //Set the index to the texture array
    m_faces.push_back(new_face);
}

void quake3_bsp_map::convert_faces() {
    typedef vector<quake3_face>::iterator quake3_face_iterator;

    //Reserve enough for 3 * num faces. it may be more or less than
    //that but it doesnt matter, we are just preventing loads
    // of mem copying
    m_faces.reserve(m_raw_face_data.size());

    for (quake3_face_iterator face = m_raw_face_data.begin();
            face != m_raw_face_data.end(); ++face) {

        switch ((*face).type) {
        case 1:
#ifndef NDEBUG
            std::cout << ".";
#endif

            add_normal_face((*face));
            break;
        case 2:
            add_curved_surface((*face));
            break;

        default:
            continue;
        }
    }

#ifndef NDEBUG
    std::cout << std::endl;
#endif
}

void quake3_bsp_map::add_mesh_surface(const quake3_face& f) {
    shared_ptr<map_face> new_face(new basic_face());
    new_face->clear();

    //Get the triangles that make up this face
    for (int i = 0; i < f.totalMeshVertices; i += 3) {
        int idx1 = f.startVertexIndex + m_raw_face_index_data[f.meshVertexIndex + i];
        int idx2 = f.startVertexIndex + m_raw_face_index_data[f.meshVertexIndex + i + 2];
        int idx3 = f.startVertexIndex + m_raw_face_index_data[f.meshVertexIndex + i + 1];

        //Add the triangle to the new face
        new_face->add_triangle(m_vertices[idx1], m_vertices[idx2], m_vertices[idx3]);
    }

    new_face->set_texture_index(f.texID); //Set the index to the texture array
    m_faces.push_back(new_face);
}

void quake3_bsp_map::convert_and_load_textures() {
    typedef vector<quake3_texture>::iterator quake3_texture_iterator;

    vector<string> extensions;
    extensions.push_back(".tga");
    extensions.push_back(".jpg");

    m_textures.resize(m_raw_texture_data.size());

    int i = 0;
    for (quake3_texture_iterator texture = m_raw_texture_data.begin();
            texture != m_raw_texture_data.end(); ++texture) {

        string filename_to_check = (*texture).file;
        //We search the listed extensions above to find an available file,
        //If the file is not available loading will fail (it will look for a file
        //without an extension). This is fine.
        for (vector<string>::const_iterator ext = extensions.begin();
                ext != extensions.end(); ++ext) {

            if (get_owning_resource_manager()->is_file_available(filename_to_check + (*ext))) {
                filename_to_check += (*ext);
                break;
            }
        }
        shared_ptr<boost::mutex> new_mutex(new boost::mutex());
        shared_ptr<resource_interface> new_texture(new devil_texture());

        //Load the textures asynchronously
        resource_id id = get_owning_resource_manager()->queue_file_for_loading(
                             filename_to_check, new_texture, new_mutex);

        m_textures[i++].set_resource_id(id);
        //TODO: copy other texture stuff across
    }
}

void quake3_bsp_map::convert_bsp_data() {
    m_bsp_data.nodes.resize(m_raw_bsp_node_data.size());

    int i = 0;
    for (vector<quake3_bsp_node>::const_iterator it = m_raw_bsp_node_data.begin();
            it != m_raw_bsp_node_data.end(); ++it) {

        m_bsp_data.nodes[i].m_children[BNI_FRONT] = (*it).frontIndex;
        m_bsp_data.nodes[i].m_children[BNI_BACK] = (*it).backIndex;

        m_bsp_data.nodes[i].m_mins[0] = (*it).aabbMin.x;
        m_bsp_data.nodes[i].m_mins[1] = (*it).aabbMin.y;
        m_bsp_data.nodes[i].m_mins[2] = (*it).aabbMin.z;

        m_bsp_data.nodes[i].m_maxs[0] = (*it).aabbMax.x;
        m_bsp_data.nodes[i].m_maxs[1] = (*it).aabbMax.y;
        m_bsp_data.nodes[i].m_maxs[2] = (*it).aabbMax.z;

        m_bsp_data.nodes[i].m_plane_index = (*it).planeIndex;

        ++i;
    }

    m_bsp_data.clusters.size = m_raw_cluster_data.size;
    m_bsp_data.clusters.bitSet.assign(m_raw_cluster_data.bitSet.begin(), m_raw_cluster_data.bitSet.end());
    m_bsp_data.clusters.totalClusters = m_raw_cluster_data.totalClusters;
}
