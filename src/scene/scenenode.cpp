
scene_node::scene_node(scene_node* parent, scene_manager* mgr):
m_parent_node(0),
m_scene_manager(0),
m_node_type(SNT_UNKNOWN) {

	assert(mgr); //A scene manager must be specified

	m_scene_manager = mgr;

	//If we have a parent node then attach this node to it
	if (parent) {
    set_parent(parent);
    parent->add_child(this);
	}

	//Update the transformation
	update_absolute_transformation();

}

scene_node::~scene_node() {
	/* when we destroy the scenenode we remove the children */
	destroy_children();
}

void scene_node::add_child(scene_node_interface* child) {
    assert(child);

    child->set_parent(this);
    m_child_nodes.push_back(child);
}

//Set the parent of this node
void scene_node::set_parent(scene_node_interface* parent) {
    m_parent_node = parent;
}

void scene_node::update_absolute_position() {
	//If this node has a parent
	if (m_parent_node) {
		//Then the absolute transformation will be the parent absolute multiplied by the relative
		Mat4 parent_absolute = m_parent_node->get_absolute_transformation()
		m_absolute_transformation = parent_absolute * get_relative_transformation()
	} else {
		//If no parent, relative == absolute
		m_absolute_transformation = get_relative_transformation()
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
