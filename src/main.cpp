#include <iostream>
#include <boost/filesystem.hpp>
#include <GL/gl.h>
#include <GL/glu.h>
#include "scene/camera.h"

#include "core/engineinterface.h"
#include "resources/resourcemanager.h"

using std::cout;
using std::endl;

using boost::filesystem::path;

//#include <gluie/gluie.h>


class App : public IEventHandler {
	public:
		App() {

	//		m_Progress = shared_ptr<gluie::ProgressBar> ( new gluie::ProgressBar(NULL, gluie::Pos(100, 400), gluie::Size(500, 25)));
			memset(&m_Keys, 0, sizeof(bool) * SDLK_LAST);
		}

		bool onEvent(const SDL_Event& e) {
			switch(e.type) {
				case SDL_VIDEORESIZE:
					resize(e.resize.w, e.resize.h);
					return true;
				break;

				case SDL_KEYDOWN:
					m_Keys[e.key.keysym.sym] = true;
					return true;

				case SDL_KEYUP:
					m_Keys[e.key.keysym.sym] = false;
					return true;
				break;
			}

			return false;
		}

		void setScene(shared_ptr<SceneManager> s) {
			scene = s;
		}

		void update() {
			scene->getActiveCamera()->setFixedYawAxis(true, Vec3(0, 1, 0));

			if (m_Keys[SDLK_LEFT]) {
				scene->getActiveCamera()->yaw(5.0f);
			}

			if (m_Keys[SDLK_RIGHT]) {
				scene->getActiveCamera()->yaw(-5.0f);
			}

			if (m_Keys[SDLK_UP]) {
				scene->getActiveCamera()->pitch(5.0f);
			}

			if (m_Keys[SDLK_DOWN]) {
				scene->getActiveCamera()->pitch(-5.0f);
			}

			if (m_Keys[SDLK_w]) {
				scene->getActiveCamera()->moveRelative(Vec3(0, 0, 0.4f));
			}

			if (m_Keys[SDLK_s]) {
				scene->getActiveCamera()->moveRelative(Vec3(0, 0, -0.4f));
			}
		}

		void resize(int w, int h) {
			if (h <= 0)	{
				h = 1;
			}

			glViewport(0, 0, w, h);

			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluPerspective(45.0f, (GLfloat)w / (GLfloat)h, 1.0f, 100.0f);

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
		}

		shared_ptr<SceneManager> scene;

	//	shared_ptr<gluie::ProgressBar> m_Progress;

		bool m_Keys[SDLK_LAST];
};


int main(int argc, char** argv) {
	resource_manager::initialize(argc, argv);
	Logger::setDefaultPath(path(argv[0]).remove_leaf().directory_string());

	shared_ptr<App> app(new App);

	shared_ptr<EngineInterface> engine = createEngineInterface(Vec2(640, 480), BitDepth(0), false);
	shared_ptr<IGraphicsDriver> video = engine->getGraphicsDriver();
	shared_ptr<SceneManager> scene = engine->getSceneManager();

	scene->getResourceManager()->add_to_search_path("q3_elkdm2.pk3");
	scene->getResourceManager()->add_to_search_path("neotech.pk3");

	scene->setWorldGeometry("maps/q3_elkdm2.bsp");

	engine->getWindow()->setWindowCaption("KazEngine Test");
	engine->addEventHandler(app);

	app->setScene(scene);

	while(engine->run()) {
		app->update();

		video->beginScene();
			scene->update();
		video->endScene();
	}

	resource_manager::deinitialize();

	return 0;
}
