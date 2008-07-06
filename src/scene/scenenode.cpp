#include "scenenode.h"
#include "iscenemanager.h"
#include "utilities/customexcept.h"

scene_node::scene_node(scene_node_interface* parent, scene_manager_interface* mgr):
m_parent_node(0),
m_scene_manager(0),
m_node_type(SNT_UNKNOWN),
m_is_visible(false),
m_is_dead(false) {

	assert(mgr); //A scene manager must be specified

	m_scene_manager = mgr;

	//If we have a parent node then attach this node to it
	if (parent) {
    set_parent(parent);
    parent->add_child(this);
	}

	//Update the transformation
	update_absolute_position();

}

/**
	Removes a child scene node, used by detach to remove
	a child from its parent
*/
void scene_node::remove_child(const scene_node_interface* child) {
	//Find the child node if it exists
	scene_node_iterator i = m_child_nodes.begin();
	for (; i != m_child_nodes.end(); ++i) {
		if ((*i) == child) {
			break;
		}
	}

	//If it doesnt exist throw an exception
	if (i == m_child_nodes.end()) {
		throw std::logic_error("Attempted to remove an invalid child node");
	}

	//Remove the child node
	m_child_nodes.erase(i);
}

/**
	Detaches this node from its parent node.
*/
void scene_node::detach() {
	scene_node_interface* parent = get_parent();
	if (parent) {
		parent->remove_child(this);
	}
}

scene_node::~scene_node() {

	detach(); //Remove from the parent node (if any, root node if this isn't the root itself)

	/* when we destroy the scenenode we remove the children */
	destroy_children();
}

/**
	Add a child node to this node
*/
void scene_node::add_child(scene_node_interface* child) {
    assert(child);

    child->set_parent(this);
    m_child_nodes.push_back(child);
}

void scene_node::update_absolute_position() {
	//If this node has a parent
	if (m_parent_node) {
		//Then the absolute transformation will be the parent absolute multiplied by the relative
		Mat4 parent_absolute = m_parent_node->get_absolute_transformation();
		m_absolute_transformation = parent_absolute * get_relative_transformation();
	} else {
		//If no parent, relative == absolute
		m_absolute_transformation = get_relative_transformation();
	}
}

void scene_node::destroy_children() {
	/*
		When we destroy the children, we mark them as dead in the scenemanager
		and then call scene_manager->flush() to actually remove them
	*/
	scene_node_iterator child = m_child_nodes.begin();
	for (; child != m_child_nodes.end(); ++child) {
		get_scene_manager()->kill_scene_node(*child);
	}

	//Actually destroy the nodes
	get_scene_manager()->flush();
}

scene_node_type scene_node::get_type() const {
	return m_node_type;
}

const Vec3 scene_node::get_position() const {
	return m_relative_translation;
}

const Vec3 scene_node::get_rotation() const {
	return m_relative_rotation;
}

const Vec3 scene_node::get_scale() const {
	return m_relative_scaling;
}

const Mat4 scene_node::get_relative_transformation() const {

	Mat4 relative_transform;

	//Set the translation of the matrix
	kmMat4Translation(&relative_transform, m_relative_translation.x,
						m_relative_translation.y, m_relative_translation.z);

	//Set the rotation
	kmMat4RotationPitchYawRoll(&relative_transform, m_relative_rotation.x,
						m_relative_rotation.y, m_relative_rotation.z);

	//TODO: Need to add relative_scaling to the relative transform!

	return relative_transform;
}
