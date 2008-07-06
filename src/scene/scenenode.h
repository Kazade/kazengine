#ifndef SCENENODE_H_INCLUDED
#define SCENENODE_H_INCLUDED

#include "iscenenode.h"

class scene_manager;

class scene_node : public scene_node_interface {
    public:
        scene_node(scene_node_interface* parent, scene_manager_interface* mgr);
				virtual ~scene_node();

				scene_node_type get_type() const;

				scene_node_interface* get_parent() { return m_parent_node; }

				//By default, do nothing with these actions
				void on_create_scene_node() {}
				void on_destroy_scene_node() {}
				void on_pre_render() {}
				void on_post_render() {}
				void render(){}
				void on_animate(float dT) {}

				const Vec3 get_position() const;
				const Vec3 get_rotation() const;
				const Vec3 get_scale() const;

				void set_visible(bool is_visible) {
					m_is_visible = is_visible;
				}

				bool is_visible() const {
					return m_is_visible;
				}

				bool is_dead() const {
					return m_is_dead;
				}

				void kill() {
					m_is_dead = true;
				}

				/**
						Nodes may register themselves in the render pipeline during this call,
						precalculate the geometry which should be renderered, and prevent
						their children from being able to register themselves if they are
						clipped by simply not calling their OnRegisterSceneNode method.
						If you are implementing your own scene node, you should overwrite
						this method with an implementation code looking like this:

                if (IsVisible)
                        SceneManager->registerNodeForRendering(this);

                ISceneNode::OnRegisterSceneNode();

				*/
				virtual void on_register_scene_node() {
					if (m_is_visible) {
						scene_node_list::iterator child = m_child_nodes.begin();
						for (; child != m_child_nodes.end(); ++child) {
							(*child)->on_register_scene_node();
						}
					}
				}

				const Mat4 get_absolute_transformation() const { return m_absolute_transformation; }
				const Mat4 get_relative_transformation() const { return m_relative_transformation; }

    private:
        scene_node_interface* m_parent_node;
        scene_manager_interface* m_scene_manager;

        list<scene_node_interface*> m_child_nodes;

        Mat4 m_absolute_transformation;
        Mat4 m_relative_transformation;

        void destroy_children();
        bool m_is_visible;
				bool m_is_dead;


		protected:
        scene_node_type m_node_type;

        void update_absolute_position();
        scene_manager_interface* get_scene_manager() const { return m_scene_manager; }

				void set_parent(scene_node_interface* parent) {
					m_parent_node = parent;
				}

				void add_child(scene_node_interface* child);
};

#endif // SCENENODE_H_INCLUDED
