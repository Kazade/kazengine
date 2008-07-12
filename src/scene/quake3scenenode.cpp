#include <tr1/memory>
#include "resources/resource_manager.h"
#include "iscenemanager.h"
#include "quake3scenenode.h"
#include "map/loaders/quake3_bsp_map.h"
#include "map/renderers/basic_map_renderer.h"

using std::tr1::dynamic_pointer_cast;

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
	bool result = true;

	shared_ptr<resource_interface> q3_map(new quake3_bsp_map());
	shared_ptr<boost::mutex> map_mutex(new boost::mutex);

	//Load the quake map
	resource_id id = rmgr->load_resource<quake3_bsp_map> (filename);

	if (rmgr->get_resource_load_status(id) != FILE_LOAD_SUCCESS) {
		result = false;
	}

	//TODO: Load textures here

	if (result) {
		m_map = dynamic_pointer_cast<base_map>(rmgr->get_resource(id));
		m_map_renderer = shared_ptr<map_renderer_interface> (new basic_map_renderer());
		m_map_renderer->initialize(m_map);
	}

	return result;
}

void quake3_scene_node::on_pre_render() {
	m_map_renderer->pre_render(shared_ptr<frustum>(), 0);
}

void quake3_scene_node::render() {
	m_map_renderer->render_map();
}
