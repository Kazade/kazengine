#include "resources/resource_manager.h"
#include "iscenemanager.h"
#include "quake3scenenode.h"
#include "map/loaders/quake3_bsp_map.h"

quake3_scene_node::quake3_scene_node(scene_node_interface* parent, scene_manager_interface* mgr):
scene_node(parent, mgr) {
	m_node_type = SNT_QUAKE3_BSP;
}

void quake3_scene_node::on_register_scene_node() {
	if (is_visible()) {
		get_scene_manager()->register_node_for_rendering(this);
	}

	scene_node::on_register_scene_node();
}

void quake3_scene_node::set_map_renderer(shared_ptr<map_renderer_interface> renderer) {
	m_map_renderer = renderer;
}

bool quake3_scene_node::load_map(shared_ptr<resource_manager> rmgr, const string& filename) {
	//Queue a file for loading asyncronously
	assert(!m_map);

	m_map_mutex.reset(new boost::mutex);

	//Create space for the map
	m_map = shared_ptr<base_map> (new quake3_bsp_map());

	shared_ptr<resource_interface> res = m_map;
	rmgr->queue_file_for_loading(filename, res, m_map_mutex);

	/*
		The status of the load can be found by calling:
		if (rmgr->get_resource_load_status(m_map->get_resource_id()) != RS_LOAD_SUCCESS) {

		}
	*/
	return true;
}
