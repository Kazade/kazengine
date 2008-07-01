#include <SDL/SDL.h>

#include "engineinterface.h"
#include "opengl2driver.h"
#include "sdlwindow.h"

/**
	Creates a new engine interface. This spawns the Window and creates a scenemanager
	and a video driver.
*/

shared_ptr<EngineInterface> createEngineInterface(const Vec2& dimensions,
													const BitDepth& bits, bool fullscreen,
													bool stencilBuffer, bool enableVsync) {

	shared_ptr<EngineInterface> newEngine(new EngineInterface);

	if(SDL_Init(SDL_INIT_VIDEO) != 0) {
		throw std::runtime_error("Could not initialize SDL");
	}

	shared_ptr<IWindow> window = shared_ptr<IWindow>(new SDLWindow);
	window->setLog(Logger::getLogger("window"));

	if (!window->create(dimensions, bits, fullscreen)) {
		throw std::runtime_error("Could not create window");
	}

	newEngine->setWindow(window);
	newEngine->addEventHandler(window);

	//FIXME: Must create this dynamically so we can switch to 3 at some point
	shared_ptr<IGraphicsDriver> videoDriver(new OpenGL2Driver);
	shared_ptr<SceneManager> sceneManager(new SceneManager);

	if (!videoDriver->initialize()) {
		throw std::runtime_error("Could not initialize the video driver");
	}

	newEngine->setGraphicsDriver(videoDriver);
	newEngine->setSceneManager(sceneManager);

	//Create a default camera
	sceneManager->createCamera("default");
	sceneManager->setActiveCamera("default");

	return newEngine;
}

EngineInterface::EngineInterface():
m_IsDone(false) {
	m_Logger = Logger::getLogger("engine");

	m_Logger->setLevel(CURRENT_LEVEL);
}

EngineInterface::~EngineInterface() {
	SDL_Quit();
}

bool EngineInterface::setSceneManager(shared_ptr<SceneManager> sceneManager) {
	if (!sceneManager) {
		return false;
	}

	m_SceneManager = sceneManager;
	return true;
}

bool EngineInterface::setGraphicsDriver(shared_ptr<IGraphicsDriver> videoDriver) {
	if (!videoDriver) {
		return false;
	}

	m_GraphicsDriver = videoDriver;

	return true;
}

bool EngineInterface::setWindow(shared_ptr<IWindow> window) {
	if (!window) {
		return false;
	}

	m_Window = window;

	return true;
}

/*bool EngineInterface::setGuiEnvironment(shared_ptr<GuiEnvironment> gui) {
	m_GUIEnvironment = gui;

	return true;
}*/


bool EngineInterface::run() {
	SDL_Event e;

	while (!m_IsDone && SDL_PollEvent(&e)) {
		switch(e.type) {
			case SDL_QUIT:
				m_IsDone = true;
				removeEventHandler(m_Window);
				m_Window->destroy();
			break;

			default:
				for (EventHandlerList::iterator handler = m_EventHandlers.begin(); handler != m_EventHandlers.end(); ++handler) {
					bool result = (*handler)->onEvent(e);
					if (result) { //If the event was handled we leave this loop
						break;
					}
				}
			break;
		}
	}

	return !m_IsDone;
}

void EngineInterface::addEventHandler(shared_ptr<IEventHandler> handler) {
	getLogger()->debug("Adding event handler");
	m_EventHandlers.insert(m_EventHandlers.end(), handler);
}

void EngineInterface::removeEventHandler(shared_ptr<IEventHandler> handler) {
	getLogger()->debug("Removing event handler");

	EventHandlerList::size_type size = m_EventHandlers.size();

	m_EventHandlers.remove(handler);

	if (size == m_EventHandlers.size()) {
		throw std::logic_error("Tried to remove non-existant event handler");
	}

}

