#ifndef SKYBOX_H_INCLUDED
#define SKYBOX_H_INCLUDED

#include "scene/entity.h"

namespace engine {

	namespace scene {

		static const float SKYBOX_SIZE = 200.0f;

		class Skybox : public Entity {
			public:
				Skybox();
				virtual ~Skybox();

				bool loadSkybox(const string& path);
				virtual void render(shared_ptr<Camera> activeCamera = shared_ptr<Camera>());
		};
	}
}

#endif // SKYBOX_H_INCLUDED
