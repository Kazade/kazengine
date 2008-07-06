#ifndef SCENENODE_H_INCLUDED
#define SCENENODE_H_INCLUDED

#include "iscenenode.h"

class scene_node : public scene_node_interface {
    public:
        scene_node(scene_node* parent, scene_manager* mgr);

				scene_node_type get_type() const;


				//By default, do nothing with these actions
				void on_create_scene_node() {}
				void on_destroy_scene_node() {}

				void set_visible(bool is_visible) {
					m_is_visible = is_visible;
				}

				bool is_visible() const {
					return m_is_visible;
				}

				virtual void on_register_scene_node() {
					if (m_is_visible) {
						scene_node_list::iterator child = m_child_nodes.begin();
						for (; child != m_child_nodes.end(); ++child) {
							child->on_register_scene_node();
						}
					}
				}
    private:
        void add_child(scene_node_interface* child);

        scene_node_interface* m_parent_node;
        scene_manager* m_scenemanager;

        list<scene_node_interface*> m_child_nodes;

        Mat4 m_absolute_transformation;
        Mat4 m_relative_transformation;

        void destroy_children();

        scene_node_type m_node_type;
};

#endif // SCENENODE_H_INCLUDED
