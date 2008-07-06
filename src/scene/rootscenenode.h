#ifndef ROOTSCENENODE_H_INCLUDED
#define ROOTSCENENODE_H_INCLUDED

#include "scenenode.h"

/**
	A class to represent the root scene node
	which always exists but is never rendered
*/
class root_scene_node : public scene_node {
	public:
		root_scene_node(scene_manager_interface* smgr):
		scene_node(0, smgr) {	}
};

#endif // ROOTSCENENODE_H_INCLUDED
