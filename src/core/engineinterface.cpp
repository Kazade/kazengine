#include <SDL/SDL.h>

#include "engineinterface.h"
#include "opengl2driver.h"
#include "sdlwindow.h"

/**
	Creates a new engine interface. This spawns the Window and creates a scenemanager
	and a video driver.
*/

shared_ptr<engine_interface> createEngineInterface(const Vec2& dimensions,
        const BitDepth& bits, bool fullscreen, bool stencilBuffer, bool enableVsync) {

    shared_ptr<engine> newEngine(new engine());

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE) != 0) {
        throw std::runtime_error("Could not initialize SDL");
    }

    shared_ptr<IWindow> window = shared_ptr<IWindow>(new SDLWindow());
    window->setLog(Logger::getLogger("window"));

    if (!window->create(dimensions, bits, fullscreen)) {
        throw std::runtime_error("Could not create window");
    }

    newEngine->setWindow(window);
    newEngine->add_event_handler(window);

    //FIXME: Must create this dynamically so we can switch to 3 at some point
    shared_ptr<graphics_driver_interface> videoDriver(new opengl_2_driver());
    shared_ptr<scene_manager_interface> sceneManager(new scene_manager(videoDriver));
    sceneManager->initialize();

    if (!videoDriver->initialize()) {
        throw std::runtime_error("Could not initialize the video driver");
    }

    newEngine->setGraphicsDriver(videoDriver);
    newEngine->setSceneManager(sceneManager);

    //Give the scene manager a pointer to it's parent
    sceneManager->set_parent_engine(newEngine);

    //Create a default camera
    scene_node_interface* cam = sceneManager->add_built_in_scene_node(SNT_CAMERA);
    sceneManager->set_active_camera(node_down_cast<camera_scene_node_interface> (cam));

    return newEngine;
}

engine::engine():
        m_graphics_driver(shared_ptr<graphics_driver_interface>()),
        m_scene_manager(shared_ptr<scene_manager_interface>()),
        m_window(shared_ptr<IWindow>()),
        m_logger(shared_ptr<Logger>()),
        m_IsDone(false) {
    m_logger = Logger::getLogger("engine");

    m_logger->setLevel(CURRENT_LEVEL);
    m_timer = shared_ptr<timer_interface>(new timer());
    m_timer->initialize();
}

engine::~engine() {
    SDL_Quit();
}

float engine::get_time_diff() {
    return m_timer->get_elapsed_time();
}

bool engine::setSceneManager(shared_ptr<scene_manager_interface> sceneManager) {
    if (!sceneManager) {
        return false;
    }

    m_scene_manager = sceneManager;
    return true;
}

bool engine::setGraphicsDriver(shared_ptr<graphics_driver_interface> videoDriver) {
    if (!videoDriver) {
        return false;
    }

    m_graphics_driver = videoDriver;

    return true;
}

bool engine::setWindow(shared_ptr<IWindow> window) {
    if (!window) {
        return false;
    }

    m_window = window;

    return true;
}

/*bool engine::setGuiEnvironment(shared_ptr<GuiEnvironment> gui) {
	m_GUIEnvironment = gui;

	return true;
}*/


bool engine::run() {
    SDL_Event e;

    while (!m_IsDone && SDL_PollEvent(&e)) {
        switch (e.type) {
        case SDL_QUIT:
            m_IsDone = true;
            remove_event_handler(m_window);
            m_window->destroy();
            break;
        default:
            bool event_handled = false;
            EventHandlerList::iterator handler = m_EventHandlers.begin();
            while (!event_handled && handler != m_EventHandlers.end()) {
                event_handled = (*handler)->on_event_received(e);
                ++handler;
            }
            break;
        }
    }

    return !m_IsDone;
}

bool engine::add_event_handler(shared_ptr<event_handler_interface> handler) {
    get_logger()->debug("Adding event handler");
    m_EventHandlers.insert(m_EventHandlers.end(), handler);
    return true;
}

void engine::remove_event_handler(shared_ptr<event_handler_interface> handler) {
    get_logger()->debug("Removing event handler");

    EventHandlerList::size_type size = m_EventHandlers.size();

    m_EventHandlers.remove(handler);

    if (size == m_EventHandlers.size()) {
        throw std::logic_error("Tried to remove non-existant event handler");
    }

}

