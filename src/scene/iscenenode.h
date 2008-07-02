#ifndef SCENENODEINTERFACE_H_INCLUDED
#define SCENENODEINTERFACE_H_INCLUDED

#include <vector>
#include <map>
#include <string>
#include <tr1/memory>
#include <kazmathxx/mat4.h>
#include <kazmathxx/vec3.h>
#include <kazmathxx/aabb.h>

enum scene_node_type {
	SNT_MAP_SCENE_NODE = 0,
	SNT_CUBE_SCENE_NODE,
	SNT_UNKNOWN
};

class scene_node_interface {
	public:
		virtual ~scene_node_interface(){}

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

		virtual void destroy() = 0;
		virtual void destroy_children() = 0;

    private:
		virtual void add_child(scene_node_interface* child) = 0;
		virtual void set_parent(scene_node_interface* parent) = 0;
};

#endif // SCENENODE_H_INCLUDED
