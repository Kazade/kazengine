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

typedef list<shared_ptr<IEventHandler> > EventHandlerList;

class EngineInterface {
	public:
		virtual ~EngineInterface();

		const shared_ptr<graphics_driver_interface> getGraphicsDriver() { return m_GraphicsDriver; }
		const shared_ptr<scene_manager_interface> getSceneManager() { return m_SceneManager; }
		const shared_ptr<IWindow> getWindow() { return m_Window; }
		const shared_ptr<Logger> getLogger() { return m_Logger; }

		bool run();

		void addEventHandler(shared_ptr<IEventHandler> handler);
		void removeEventHandler(shared_ptr<IEventHandler> handler);
	protected:

	private:
		EngineInterface(); //Only createEngineInterface can create a new Engine

		shared_ptr<graphics_driver_interface> m_GraphicsDriver;
		shared_ptr<scene_manager_interface> m_SceneManager;
		shared_ptr<IWindow> m_Window;
		shared_ptr<Logger> m_Logger;
//		shared_ptr<GuiEnvironment> m_GUIEnvironment;

		//Only createEngineInterface can call these methods
		bool setSceneManager(shared_ptr<scene_manager_interface> sceneManager);
		bool setGraphicsDriver(shared_ptr<graphics_driver_interface> videoDriver);
		bool setWindow(shared_ptr<IWindow> window);
	//	bool setGuiEnvironment(shared_ptr<GuiEnvironment> gui);

		bool m_IsDone;

		EventHandlerList m_EventHandlers;

	friend shared_ptr<EngineInterface> createEngineInterface(const Vec2& dimensions,
																													const BitDepth& bits,
																													bool fullscreen,
																													bool stencilBuffer,
																													bool enableVsync);
};



extern shared_ptr<EngineInterface> createEngineInterface(const Vec2& dimensions,
																													const BitDepth& bits,
																													bool fullscreen=false,
																													bool stencilBuffer=true,
																													bool enableVsync=false);

#endif // ENGINEINTERFACE_H_INCLUDED
