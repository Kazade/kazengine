#ifndef ENTITY_H_INCLUDED
#define ENTITY_H_INCLUDED

//#include <sheeplib/model/model.h>
#include <tr1/memory>
#include <string>

#include "resources/resourcemanager.h"

using std::tr1::shared_ptr;
using std::string;

namespace engine {

	namespace scene {

    class Camera;

		class Entity {
			private:
				//shared_ptr<Model> m_Mesh;
				string m_Name;

				shared_ptr<resource_manager> m_ResourceManager;

			public:
				Entity(const string& name);
				virtual ~Entity();

				//shared_ptr<Model> getMesh();
				shared_ptr<Entity> clone(const string& newName); ///< Create a clone of this entity and return a pointer

				string getName();
				void useMesh(const string& mesh);

				virtual void render(shared_ptr<Camera> activeCamera = shared_ptr<Camera>());

				void setTextureManager(shared_ptr<resource_manager> tex) {
					m_ResourceManager = tex;
				}

				shared_ptr<resource_manager> getTextureManager() {
					return m_ResourceManager;
				}

		};
	}
}

#endif // ENTITY_H_INCLUDED
