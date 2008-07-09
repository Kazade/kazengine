#include <iostream>
#include <boost/filesystem.hpp>
#include <GL/gl.h>
#include <GL/glu.h>
#include "scene/camera.h"
#include "scene/imapscenenode.h"
#include "core/engineinterface.h"
#include "resources/resource_manager.h"

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

		void setScene(shared_ptr<scene_manager_interface> s) {
			scene = s;
		}

		void update() {
//			scene->get_active_camera()->set_fixed_axis(Vec3(0, 1, 0));
//			scene->get_active_camera()->enable_fixed_axis(true);

			if (m_Keys[SDLK_LEFT]) {
				scene->get_active_camera()->yaw(5.0f);
			}

			if (m_Keys[SDLK_RIGHT]) {
				scene->get_active_camera()->yaw(-5.0f);
			}

			if (m_Keys[SDLK_UP]) {
				scene->get_active_camera()->pitch(5.0f);
			}

			if (m_Keys[SDLK_DOWN]) {
				scene->get_active_camera()->pitch(-5.0f);
			}

			if (m_Keys[SDLK_w]) {
				scene->get_active_camera()->move_relative(Vec3(0, 0, 0.4f));
			}

			if (m_Keys[SDLK_s]) {
				scene->get_active_camera()->move_relative(Vec3(0, 0, -0.4f));
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

		shared_ptr<scene_manager_interface> scene;

	//	shared_ptr<gluie::ProgressBar> m_Progress;

		bool m_Keys[SDLK_LAST];
};


int main(int argc, char** argv) {
	resource_manager::initialize(argc, argv);
	Logger::setDefaultPath(path(argv[0]).remove_leaf().directory_string());

	shared_ptr<App> app(new App());

	shared_ptr<EngineInterface> engine = createEngineInterface(Vec2(640, 480), BitDepth(0), false);
	shared_ptr<IGraphicsDriver> video = engine->getGraphicsDriver();
	shared_ptr<scene_manager_interface> scene = engine->getSceneManager();

	scene->get_resource_manager()->add_to_search_path("q3_elkdm2.pk3");
	scene->get_resource_manager()->add_to_search_path("neotech.pk3");

	//Create a new map scene node, how can we tie this into the resource manager?
	scene_node_interface* mapnode = scene->add_built_in_scene_node(SNT_QUAKE3_BSP);
	(dynamic_cast<map_scene_node_interface*>(mapnode))->load_map(scene->get_resource_manager(), "maps/q3_elkdm2.bsp");

	//TODO: Load the map
//	scene->setWorldGeometry("maps/q3_elkdm2.bsp");

	engine->getWindow()->setWindowCaption("KazEngine Test");
	engine->addEventHandler(app);

	app->setScene(scene);

	while(engine->run()) {
		app->update();

		video->beginScene();
			scene->render_all();
		video->endScene();
	}

	resource_manager::deinitialize();

	return 0;
}
