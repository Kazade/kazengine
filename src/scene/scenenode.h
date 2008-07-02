#ifndef SCENENODE_H_INCLUDED
#define SCENENODE_H_INCLUDED

#include "iscenenode.h"

class scene_node : public scene_node_interface {
    public:
        scene_node(scene_node* parent, scene_manager* mgr);

    private:
        void add_child(scene_node_interface* child);

        scene_node_interface* m_parent_node;
        scene_manager* m_scenemanager;

        list<scene_node_interface*> m_child_nodes;
};

#endif // SCENENODE_H_INCLUDED
