#ifndef FPSCAMERASCENENODE_H_INCLUDED
#define FPSCAMERASCENENODE_H_INCLUDED

#include "icamerascenenode.h"
#include "core/ieventhandler.h"
#include "camera.h"
/**
	A scene node that is a FPS style camera
*/
class fps_camera_scene_node : public scene_node, public camera_scene_node_interface, public event_handler_interface {
public:
    fps_camera_scene_node(scene_node_interface* parent, scene_manager_interface* smgr):
            scene_node(parent, smgr),
            m_camera("fps") {
        m_camera.setFixedYawAxis(true, Vec3(0, 1, 0));

        m_key_map[CA_FORWARD].key = SDLK_UP;
        m_key_map[CA_BACKWARD].key = SDLK_DOWN;
        m_key_map[CA_LEFT].key = SDLK_LEFT;
        m_key_map[CA_RIGHT].key = SDLK_RIGHT;

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

    void on_register_scene_node() {
        if (is_visible()) {
            get_scene_manager()->register_node_for_rendering(this);
        }

        scene_node::on_register_scene_node();
    }

    bool on_event_received(const SDL_Event& e) {

        //TODO: We need to implement some kind of keymap for this
        if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP) {
            if (e.type == SDL_KEYDOWN) {
                for (uint i = 0; i < CA_MAX; ++i) {
                    if (e.key.keysym.sym == m_key_map[i].key) {
                        m_key_map[i].pressed = true;
                    }
                }
            }

            if (e.type == SDL_KEYUP) {
                for (uint i = 0; i < CA_MAX; ++i) {
                    if (e.key.keysym.sym == m_key_map[i].key) {
                        m_key_map[i].pressed = false;
                    }
                }
            }

            return false; //We want key events to propergate
        }

        return false; //Should always return false for all other events
    }

    void use(shared_ptr<graphics_driver_interface> graphics_driver) {
        m_camera.use(graphics_driver);
    }

    void on_animate(float dt) {
        const float move_speed = 5.0f;

        //FIXME: DELTATIME!
        if (m_key_map[CA_FORWARD].pressed) {
            Vec3 move(0, 0, move_speed);
            move_relative(move * dt);
        }

        if (m_key_map[CA_BACKWARD].pressed) {
            Vec3 move(0, 0, -move_speed);
            move_relative(move * dt);
        }

        if (m_key_map[CA_LEFT].pressed) {
            m_camera.yaw(move_speed * dt);
        }

        if (m_key_map[CA_RIGHT].pressed) {
            m_camera.yaw(move_speed * dt);
        }
    }

    virtual const frustum& get_frustum() {
		return m_frustum;
    }
    //void set_fixed_axis(const kmVec3& axis) = 0;
    //void enable_fixed_axis(bool enabled=true) = 0;
private:
    Camera m_camera;

    struct Key {
        Key():
                pressed(false),
                key(SDLK_UNKNOWN) {

        }
        bool pressed;
        SDLKey key;
    };

    enum camera_actions {
        CA_FORWARD = 0,
        CA_BACKWARD,
        CA_LEFT,
        CA_RIGHT,
        CA_JUMP,
        CA_MAX
    };

    Key m_key_map[CA_MAX];

    frustum m_frustum;
};

#endif // FPSCAMERASCENENODE_H_INCLUDED
