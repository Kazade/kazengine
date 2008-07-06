#ifndef SCENENODEINTERFACE_H_INCLUDED
#define SCENENODEINTERFACE_H_INCLUDED

#include <vector>
#include <map>
#include <string>
#include <list>
#include <tr1/memory>
#include <kazmathxx/mat4.h>
#include <kazmathxx/vec3.h>
#include <kazmathxx/aabb.h>

using std::list;

enum scene_node_type {
	SNT_QUAKE3_BSP = 0,
	SNT_CUBE,
	SNT_CAMERA,
	SNT_UNKNOWN
};

class scene_manager_interface;

class scene_node_interface {
	public:
		virtual ~scene_node_interface(){}

		virtual void on_create_scene_node() = 0;
		virtual void on_destroy_scene_node() = 0;

		virtual void on_register_scene_node() = 0;
		virtual void on_pre_render() = 0;
		virtual void on_post_render() = 0;
		virtual void render() = 0;
		virtual void on_animate(float dT) = 0;

		virtual scene_node_interface* get_parent() = 0;

		virtual const Vec3 get_position() const = 0;
		virtual const Vec3 get_rotation() const = 0;
		virtual const Vec3 get_scale() const = 0;

		virtual scene_node_type get_type() const = 0;

		virtual void destroy_children() = 0;
		virtual void detach() = 0;
		virtual void remove_child(const scene_node_interface* child) = 0;

		virtual const Mat4 get_absolute_transformation() const = 0;
		virtual const Mat4 get_relative_transformation() const = 0;

		virtual bool is_dead() const = 0; //Return true if the scene node is waiting to be destroyed
		virtual void kill() = 0;

		virtual void set_visible(bool is_visible) = 0;
		virtual bool is_visible() const = 0;

		virtual void add_child(scene_node_interface* child) = 0;
		virtual void set_parent(scene_node_interface* parent) = 0;
		virtual void update_absolute_position() = 0;
		virtual scene_manager_interface* get_scene_manager() const = 0;
};

typedef list<scene_node_interface*> scene_node_list;
typedef scene_node_list::iterator scene_node_iterator;

#endif // SCENENODE_H_INCLUDED
