#ifndef IENGINE_H_INCLUDED
#define IENGINE_H_INCLUDED

#include <tr1/memory>

using std::tr1::shared_ptr;

class event_handler_interface;
class scene_manager_interface;
class graphics_driver_interface;
class Logger;
class IWindow;

class engine_interface {
    public:
        virtual ~engine_interface() {}

        virtual bool add_event_handler(shared_ptr<event_handler_interface> handler) = 0;
        virtual void remove_event_handler(shared_ptr<event_handler_interface> handler) = 0;

        virtual const shared_ptr<Logger> get_logger() = 0;
        virtual const shared_ptr<scene_manager_interface> get_scene_manager() = 0;
        virtual const shared_ptr<graphics_driver_interface> get_graphics_driver() = 0;

        virtual const shared_ptr<IWindow> get_window() = 0;

        virtual bool run() = 0;
};


#endif // IENGINE_H_INCLUDED
