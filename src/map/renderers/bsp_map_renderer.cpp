#include <iostream>
#include <tr1/memory>

#include "../bsp_map.h"
#include "bsp_map_renderer.h"

using std::tr1::dynamic_pointer_cast;
bool bsp_map_renderer::initialize(shared_ptr<base_map> map) {
    shared_ptr<bsp_map> bsp_map_instance = dynamic_pointer_cast<bsp_map>(map);
    if (!bsp_map_instance) {
#ifndef NDEBUG
        std::cout << "This is not a bsp map!" << std::endl;
#endif
        return false;
    }

    m_map = bsp_map_instance;

    return true;
}

/** Generate any resources that require a valid OGL context */
void bsp_map_renderer::generate_resources() {

}

/** Perform any prerendering steps using the frustum for any culling operations */
void bsp_map_renderer::pre_render(shared_ptr<frustum> frustum) {

}

void bsp_map_renderer::post_render() {

}

void bsp_map_renderer::render_map() {

}

int bsp_map_renderer::get_camera_leaf(const Vec3& camera_position) {
    int i = 0, result = 0;

    //Get a reference to the nodes and planes of this map
    vector<bsp_node>& nodes = m_map->get_bsp_data().nodes;
    vector<Plane>& planes = m_map->get_planes();

    // Loop through all the nodes until we find one that is a negative
    // number.  That negative number means we are in a leaf.
    while (i >= 0) {
        // Get if the point is in front or behind the plane.
        result = planes[nodes[i].m_plane_index].classifyPoint(camera_position);

        // Test if the camera is in front or behind the plane.
        if (result == POINT_INFRONT_OF_PLANE) {
            i = nodes[i].m_children[BNI_FRONT];
        } else {
            i = nodes[i].m_children[BNI_BACK];
        }
    }

    // Since we can't have a negative index, make it positive.
    return ~i;
}

bool bsp_map_renderer::is_cluster_visible(int cluster_1_id, int cluster_2_id)  {
    if (m_map->get_bsp_data().clusters.bitSet.empty()) {
        return true;
    }

    if (cluster_1_id < 0) {
        return true;
    }


    uint cluster_size = m_map->get_bsp_data().clusters.size;

    // Get the 8 bit visiblity for this cluster and the one we are testing for.
    unsigned char set = m_map->get_bsp_data().clusters.bitSet[(cluster_1_id * cluster_size) +
                        (cluster_2_id / 8)];

    // Return if this set is visible or not.  We bit shift to get
    // either true or false.
    return set & (1 << (cluster_2_id & 7));
}
