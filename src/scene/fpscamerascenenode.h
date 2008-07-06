#ifndef FPSCAMERASCENENODE_H_INCLUDED
#define FPSCAMERASCENENODE_H_INCLUDED

#include "icamerascenenode.h"
#include "camera.h"
/**
	A scene node that is a FPS style camera
*/
class fps_camera_scene_node : public scene_node, public camera_scene_node_interface {
	public:
		fps_camera_scene_node(scene_node_interface* parent, scene_manager_interface* smgr):
		scene_node(parent, smgr),
		m_camera("fps") {
			m_camera.setFixedYawAxis(true, Vec3(0, 1, 0));
		}

		void yaw(const float amount) {
			m_camera.yaw(amount);
		}

		void pitch(const float amount) {
			m_camera.pitch(amount);
		}

		void roll(const float amount) {
			m_camera.roll(amount);
		}

		void move_relative(const kmVec3& vec) {
			m_camera.moveRelative(vec);
		}

		//void set_fixed_axis(const kmVec3& axis) = 0;
		//void enable_fixed_axis(bool enabled=true) = 0;
	private:
		Camera m_camera;
};

#endif // FPSCAMERASCENENODE_H_INCLUDED
