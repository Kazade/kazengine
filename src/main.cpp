#include <iostream>
#include <boost/filesystem.hpp>
#include <GL/gl.h>
#include <GL/glu.h>
#include "core/iengine.h"
#include "scene/camera.h"
#include "scene/imapscenenode.h"
#include "core/engineinterface.h"
#include "resources/resource_manager.h"
#include "map/loaders/quake3_bsp_map.h"

using std::cout;
using std::endl;

using boost::filesystem::path;

//#include <gluie/gluie.h>


class App : public event_handler_interface {
	public:
		App() {

	//		m_Progress = shared_ptr<gluie::ProgressBar> ( new gluie::ProgressBar(NULL, gluie::Pos(100, 400), gluie::Size(500, 25)));
			memset(&m_Keys, 0, sizeof(bool) * SDLK_LAST);
		}

		bool on_event_received(const SDL_Event& e) {
			switch(e.type) {
				case SDL_VIDEORESIZE:
					resize(e.resize.w, e.resize.h);
					return true;
				break;

/*				case SDL_KEYDOWN:
					m_Keys[e.key.keysym.sym] = true;
					return true;

				case SDL_KEYUP:
					m_Keys[e.key.keysym.sym] = false;
					return true;
				break;*/
			}

			return false;
		}

		void setScene(shared_ptr<scene_manager_interface> s) {
			scene = s;
		}

		void update() {
//			scene->get_active_camera()->set_fixed_axis(Vec3(0, 1, 0));
//			scene->get_active_camera()->enable_fixed_axis(true);

		}

		void resize(int w, int h) {
			if (h <= 0)	{
				h = 1;
			}

			glViewport(0, 0, w, h);

			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluPerspective(45.0f, (GLfloat)w / (GLfloat)h, 1.0f, 1500.0f);

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

	shared_ptr<engine_interface> engine = createEngineInterface(Vec2(640, 480), BitDepth(0), false);
	shared_ptr<graphics_driver_interface> video = engine->get_graphics_driver();
	shared_ptr<scene_manager_interface> scene = engine->get_scene_manager();

	engine->get_window()->setWindowCaption("KazEngine Test");

	scene->get_resource_manager()->add_to_search_path("q3_elkdm2.pk3");
	scene->get_resource_manager()->add_to_search_path("pak1-maps.pk3");
	scene->get_resource_manager()->add_to_search_path("pak4-textures.pk3");
	scene->get_resource_manager()->add_to_search_path("map-acid3dm7.pk3");

	scene->add_quake3_scene_node("maps/q3_elkdm2.bsp");
	//scene->add_quake3_scene_node("maps/acid3dm7.bsp");

	engine->add_event_handler(app);

//	app->setScene(scene);

	//glDisable(GL_TEXTURE_2D);
	//glEnable(GL_NORMALIZE);
	//glEnable(GL_LIGHT0);
	//glEnable(GL_LIGHTING);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	float angle = 0.0f;
	while(engine->run()) {
//		app->update();

		video->begin_scene();
	//		glTranslatef(0, -3, -10);
//
			//(angle > 359.0f) ? angle -= 360.0f : ++angle;

		//	glRotatef(angle, 0, 1, 0);
			scene->render_all();
		video->end_scene();
	}

	resource_manager::deinitialize();

	return 0;
}
