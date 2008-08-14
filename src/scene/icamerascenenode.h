#ifndef ICAMERASCENENODE_H_INCLUDED
#define ICAMERASCENENODE_H_INCLUDED

#include <tr1/memory>

using std::tr1::shared_ptr;

class frustum;
class graphics_driver_interface;

class camera_scene_node_interface {
	public:
		virtual ~camera_scene_node_interface() {}

		virtual void yaw(const float amount) = 0;
		virtual void pitch(const float amount) = 0;
		virtual void roll(const float amount) = 0;

		virtual void move_relative(const kmVec3& vec) = 0;
		virtual void use(shared_ptr<graphics_driver_interface> graphics_driver) = 0;
/*		virtual void set_fixed_axis(const kmVec3& axis) = 0;
		virtual void enable_fixed_axis(bool enabled=true) = 0;*/

		virtual const frustum& get_frustum() = 0;
};

#endif // ICAMERASCENENODE_H_INCLUDED
