#include "scene/entity.h"

namespace engine {

	namespace scene {
		Entity::Entity(const string& name):
		m_Name(name) {

		}

		Entity::~Entity() {

		}

		string Entity::getName() {
			return m_Name;
		}

		void Entity::useMesh(const string& mesh) {
			//Either load the mesh or clone it
		}

		void Entity::render(shared_ptr<Camera> activeCamera) {

		}
	}
}
