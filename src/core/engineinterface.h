#ifndef ENGINEINTERFACE_H_INCLUDED
#define ENGINEINTERFACE_H_INCLUDED

#include <tr1/memory>
#include <kazmathxx/vec2.h>
#include <list>

#include "igraphicsdriver.h"
#include "utilities/logger.h"
#include "scene/scenemanager.h"
#include "iwindow.h"
#include "guienvironment.h"

using std::tr1::shared_ptr;
using std::list;

typedef list<shared_ptr<event_handler_interface> > EventHandlerList;

class engine {
	public:
		virtual ~engine();

		const shared_ptr<graphics_driver_interface> get_graphics_driver() { return m_graphics_driver; }
		const shared_ptr<scene_manager_interface> get_scene_manager() { return m_scene_manager; }
		const shared_ptr<IWindow> get_window() { return m_window; }
		const shared_ptr<Logger> get_logger() { return m_logger; }

		bool run();

		void add_event_handler(shared_ptr<event_handler_interface> handler);
		void remove_event_handler(shared_ptr<event_handler_interface> handler);
	protected:

	private:
		engine(); //Only createEngineInterface can create a new Engine

		shared_ptr<graphics_driver_interface> m_graphics_driver;
		shared_ptr<scene_manager_interface> m_scene_manager;
		shared_ptr<IWindow> m_window;
		shared_ptr<Logger> m_logger;
//		shared_ptr<GuiEnvironment> m_GUIEnvironment;

		//Only createEngineInterface can call these methods
		bool setSceneManager(shared_ptr<scene_manager_interface> sceneManager);
		bool setGraphicsDriver(shared_ptr<graphics_driver_interface> videoDriver);
		bool setWindow(shared_ptr<IWindow> window);
	//	bool setGuiEnvironment(shared_ptr<GuiEnvironment> gui);

		bool m_IsDone;

		EventHandlerList m_EventHandlers;

    friend shared_ptr<engine> createEngineInterface(const Vec2& dimensions,
	               const BitDepth& bits, bool fullscreen, bool stencilBuffer, bool enableVsync);
};


extern shared_ptr<engine> createEngineInterface(const Vec2& dimensions,
        const BitDepth& bits, bool fullscreen=false, bool stencilBuffer=true, bool enableVsync=false);

#endif // ENGINEINTERFACE_H_INCLUDED
