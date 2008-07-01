#ifndef SCENENODE_H_INCLUDED
#define SCENENODE_H_INCLUDED

#include <vector>
#include <map>
#include <string>
#include <tr1/memory>
#include <kazmathxx/mat4.h>
#include <kazmathxx/vec3.h>
#include <kazmathxx/aabb.h>

using std::map;
using std::tr1::shared_ptr;
using std::vector;
using std::string;

namespace engine {

	namespace scene {

		class Entity;
		class SceneManager;
		class OctreeSceneNode;
		class BSPSceneNode;

		class SceneNode {
			private:
				vector< shared_ptr<SceneNode> > m_Children;
				map<string, shared_ptr<Entity> > m_Objects;

				Mat4  m_Transform;
				AABB	m_Bounds;

				const SceneManager* m_Creator;
			public:
				SceneNode();
				virtual ~SceneNode();

				const uint getNumAttachedObjects() const;
				bool attachObject(shared_ptr<Entity> newObject);
				void detachObject(const string& name);
				void detachAllObjects();

				shared_ptr<Entity> getAttachedObject(const string& name);

				void translate(const Vec3& vec);
				void scale(const Vec3& vec);
				void rotate(const Vec3& vec);

				const Vec3 getPosition() const;
				void setPosition(const Vec3& vec);

				shared_ptr<SceneNode> createChildSceneNode(const string& name, Vec3 vec = Vec3());

				const AABB& getBounds() const;
				void updateBounds();

				virtual void update(shared_ptr<Camera> activeCamera = shared_ptr<Camera>());

				uint getNumChildNodes() { return m_Children.size(); }

				const SceneManager* getSceneManager() { return m_Creator; }
				void setCreator(const SceneManager* man) { m_Creator = man; }
		};
	}
}

#endif // SCENENODE_H_INCLUDED
