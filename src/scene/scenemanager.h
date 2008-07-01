#ifndef SCENEMANAGER_H_INCLUDED
#define SCENEMANAGER_H_INCLUDED

#include <map>
#include <string>
#include <tr1/memory>
#include <kazmathxx/vec4.h>

#include "resources/resourcemanager.h"

using std::tr1::shared_ptr;
using std::string;

class Map;

namespace engine {

	namespace scene {

	//	using engine::map::Q3BSP;

		class SceneNode;
		class Entity;
		class Camera;

		using std::map;

		class SceneManager {
			private:
				shared_ptr<SceneNode> m_RootSceneNode;

				map< string, shared_ptr<Entity> > m_Objects;
				map< string, shared_ptr<Camera> > m_Cameras;

				shared_ptr<resource_manager> m_ResourceManager;
				shared_ptr<Camera> m_ActiveCamera;

				bool m_DebugMode;

				//shared_ptr<RenderWindow> m_TargetWindow;

				void updateFPS();
				float m_FPS;

				shared_ptr<Map> m_Map;
			public:
				SceneManager();
				virtual ~SceneManager();

				shared_ptr<Entity> createEntity(const string& name, const string& mesh);
				shared_ptr<Camera> createCamera(const string& name);
				shared_ptr<Map> getMap() { return m_Map; }

				void setAmbientLight(const Vec4& colour);
				bool setWorldGeometry(const string& filePath);

				void setSkyBox(bool enable, const string& materialName);

				shared_ptr<SceneNode> getRootSceneNode();

				shared_ptr<Camera> getCamera(const string& name);
				bool setActiveCamera(const string& name);

				shared_ptr<Camera> getActiveCamera();

				shared_ptr<resource_manager> getResourceManager() const;

				void setDebugMode(bool val = true) { m_DebugMode = val; }
				const bool getDebugMode() { return m_DebugMode; }

				/*void setTargetWindow(shared_ptr<RenderWindow> win) {
                    m_TargetWindow = win;
				}*/

				void update();
                const float& getFPS() const { return m_FPS; }
		};
	}
}

#endif // SCENEMANAGER_H_INCLUDED
