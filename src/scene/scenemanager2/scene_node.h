#ifndef SCENE_NODE_H_INCLUDED
#define SCENE_NODE_H_INCLUDED

class scene_node_interface {
	public:
		virtual void add_child(scene_node_interface* child) = 0;
};

#endif // SCENE_NODE_H_INCLUDED
