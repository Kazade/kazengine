
scene_node::scene_node(scene_node* parent, scene_manager* mgr) {
    set_parent(parent);

    m_scene_manager = mgr
}

void scene_node::add_child(scene_node_interface* child) {
    assert(child);

    child->set_parent(this);
    m_child_nodes.push_back(child);
}

void scene_node::set_parent(scene_node_interface* parent) {
    m_parent_node = parent;
}
