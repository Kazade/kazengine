#include "scenemanager.h"
#include "rootscenenode.h"
#include "quake3scenenode.h"
#include "utilities/customexcept.h"

scene_manager::scene_manager() {
	m_root_node = shared_ptr<scene_node_interface> (new root_scene_node);
}

scene_manager::~scene_manager() {

}

void scene_manager::kill_scene_node(scene_node_interface* node) {
	scene_node_list::iterator i = get_node_iterator(node);

	if (i != m_node_list.end()) {
		(*i)->kill(); //Mark the node as dead
		m_dead_nodes.push_back((*i)); //Add to the dead nodes for removal
		return;
	}

	throw std::logic_error("Attempted to kill a non-existant node");
}


void scene_manager::flush() {
	//Get an iterator to the dead nodes
	scene_node_list::iterator i = m_dead_nodes.begin();

	//Loop through the dead nodes
	for (; i != m_dead_nodes.end(); ++i) {
		//Get an iterator in the total node list
		scene_node_list::iterator j = get_node_iterator((*i).get());
		(*j)->on_destroy_scene_node(); //Call the on_destroy event
		m_scene_nodes.erase(j); //Remove from the global list
	}

	m_dead_nodes.clear(); //Clear the dead node list
}

scene_manager_interface* scene_manager::add_built_in_scene_node(scene_node_type type, scene_node_interface* parent) {
	switch(type) {
		case SNT_QUAKE3_BSP:
			//If no parent was specified we add to the root node
			if (!parent) {
				parent = m_root_node.get();
			}
			shared_ptr<scene_node_interface> new_node(new quake3_scene_node(parent, this));
			m_scene_nodes.push_back(new_node);
			new_node->on_create_scene_node();
			return new_node.get();

		default:
			throw not_implemented_error("Attempted to spawn an invalid scenenode type");
	};
};

bool scene_manager::register_scene_node_factory(shared_ptr<scene_node_factory_interface> factory) {
	throw not_implemented_error("Factory registration is currently unimplemented");

	return false;
}

scene_node_interface* scene_manager::add_custom_scene_node(const string& type_name, scene_node_interface* parent = 0) {
	throw not_implemented_error("Custom scene node creation is unimplemented");
	return 0;
}

void scene_manager::render_all() {
	//Traverse the tree calling on_register_scene_node
	m_root_node->on_register_scene_node();

}

void scene_manager::register_node_for_rendering(scene_node_interface* node) {
	m_nodes_for_rendering.push_back(node);
}
