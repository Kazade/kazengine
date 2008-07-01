#include <boost/lexical_cast.hpp>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

#include "scene/scenemanager.h"
#include "scene/entity.h"

#include "scene/scenenode.h"
#include "utilities/logger.h"
#include "scene/skybox.h"
#include "scene/camera.h"
#include "utilities/patterns/abstractfactory.h"


#include <tr1/memory>

//#include <gluie/gluie.h>

using std::tr1::dynamic_pointer_cast;

namespace engine {

	namespace scene {

		SceneManager::SceneManager() {
			m_RootSceneNode = shared_ptr<SceneNode>(new SceneNode());
			m_ResourceManager = shared_ptr<resource_manager> (new resource_manager);
//			Texture::initialize();
			Logger::getLogger("scenemanager")->setLevel(LL_DEBUG);
		}

		SceneManager::~SceneManager() {
			m_ResourceManager.reset();
	//		Texture::deinitialize();
		}

		///	Creates a new entity for the scene, must be attached to a scene node to be
		/// renderered
		shared_ptr<Entity> SceneManager::createEntity(const string& name, const string& mesh) {
			assert(m_Objects.find(name) == m_Objects.end());

			shared_ptr<Entity> ent(new Entity(name));
			ent->useMesh(mesh);
			m_Objects[name] = ent;

			ent->setTextureManager(m_ResourceManager);
			//ent->setMaterialManager(m_MaterialManager);
			return ent;
		}

		void SceneManager::setAmbientLight(const Vec4& colour) {

		}

		void SceneManager::setSkyBox(bool enable, const string& texturePath) {
			Logger::getLogger("scenemanager")->debug("Loading skybox");
			shared_ptr<Skybox> sky(new Skybox());
			sky->setTextureManager(m_ResourceManager);

			if (!sky->loadSkybox("textures/sky")) {
				Logger::getLogger("scenemanager")->error("Skybox loading failed!");
				return;
			}

			shared_ptr<SceneNode> node = m_RootSceneNode->createChildSceneNode("sky");
			node->setCreator(this);
			node->attachObject(sky);

			Logger::getLogger("scenemanager")->debug("Rootsize: " + boost::lexical_cast<string>(m_RootSceneNode->getNumChildNodes()));
		}

		shared_ptr<SceneNode> SceneManager::getRootSceneNode() {
			return m_RootSceneNode;
		}

		void enter2dmode() {

			GLint viewport[4];					// Where The Viewport Values Will Be Stored
			glGetIntegerv(GL_VIEWPORT, viewport);			// Retrieves The Viewport Values (X, Y, Width, Height)

			glMatrixMode(GL_PROJECTION);		// Enter 2D Rendering Mode
			glPushMatrix();
			glLoadIdentity();
			glOrtho(0, viewport[2], viewport[3], 0, -1, 1);
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glLoadIdentity();
			glDisable(GL_DEPTH_TEST);			// Turn off depth testing and lighting
		}

		void leave2dmode() {
			glPopMatrix();						// Reset back to 3D Rendering Mode
			glMatrixMode(GL_PROJECTION);
			glPopMatrix();
			glMatrixMode(GL_MODELVIEW);
			glEnable(GL_DEPTH_TEST);			// Restore previous settings for depth testing and lighting
		}


		bool SceneManager::setWorldGeometry(const string& filename) {
			//TODO: Prevent this function being called twice
	/*		shared_ptr<gluie::ProgressBar> progressBar(new gluie::ProgressBar(NULL, gluie::Pos(10, 10), gluie::Size(450, 10)));
			assert(m_ResourceManager);


			//Create a new progress indicator, this is just temporary
			shared_ptr<ProgressIndicator> temp = shared_ptr<ProgressIndicator>(new ProgressIndicator);

			//Create the loading thread
			shared_ptr<ThreadedResourceLoader> loader = m_ResourceManager->loadResourceInThread<Map, Q3BSP>(filename, temp);

			//While it is loading we want to update the progress bar to reflect the progress indicator
			while(!loader->isDone()) {
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				glLoadIdentity();

				enter2dmode();
				//std::cout << ".";
				progressBar->setPercentage(temp->getPercentage());
				progressBar->draw();
				leave2dmode();

				SDL_GL_SwapBuffers();
			}

			//ResourceID mapID = m_ResourceManager->loadResource<Map, Q3BSP>(filename);
			ResourceID mapID = loader->getResourceID();

			if (mapID == -1) {
					std::cout << "Error loading map: " << filename << std::endl;
					return false;
			}

			weak_ptr<Map> wpMap = m_ResourceManager->getResource<Map>(mapID);
			shared_ptr<Map> map = wpMap.lock();
			assert(map);

			shared_ptr<Q3BSP> bsp = dynamic_pointer_cast<Q3BSP> (map);
			bsp->generateTextures();
			bsp->loadLightmaps();

			//shared_ptr<SceneNode> node = m_RootSceneNode->createChildOctreeSceneNode("geometry", Vec3(), octree);
			shared_ptr<SceneNode> node = m_RootSceneNode->createChildBSPSceneNode("geometry", Vec3(), bsp);
			node->setCreator(this);

			m_Map = map;*/

			return true;
		}

		shared_ptr<Camera> SceneManager::createCamera(const string& name) {
			if (m_Cameras.find(name) != m_Cameras.end()) {
				Logger::getLogger("scenemanager")->warning("There is already a camera with name: " + name + ". Returning that one instead of creating a new");
				return m_Cameras[name];
			}

			m_Cameras.insert(make_pair(name, shared_ptr<Camera> (new Camera(name))));
			return m_Cameras[name];
		}

		shared_ptr<Camera> SceneManager::getCamera(const string& name) {
			assert(m_Cameras.find(name) != m_Cameras.end());
			return m_Cameras[name];
		}

		bool SceneManager::setActiveCamera(const string& name) {
			if (m_Cameras.find(name) == m_Cameras.end()) {
				return false;
			}

			m_ActiveCamera = m_Cameras[name];
			return true;
		}

		shared_ptr<Camera> SceneManager::getActiveCamera() {
			return m_ActiveCamera;
		}

		shared_ptr<resource_manager> SceneManager::getResourceManager() const {
			return m_ResourceManager;
		}

		void SceneManager::update() {
				if (getActiveCamera()) {
						getActiveCamera()->use();
				}

				getRootSceneNode()->update(getActiveCamera());
				updateFPS();
		}

		void SceneManager::updateFPS() {
				static uint frames = 0;
				static float startTime = SDL_GetTicks();

				frames++;
				if ((SDL_GetTicks() - startTime) >= 1000) {
						//1000 ticks = 1 second, if 1000 ticks
						//have passed since start_time was recorded,
						//we can get the frames per second.
						m_FPS = frames;
						//reset frames counter...
						frames = 0;
						//update the start_time to the current "tick".
						startTime = SDL_GetTicks();
				}
		}

	}
}
