#include <GL/gl.h>

#include "scene/entity.h"
#include "scene/scenenode.h"

namespace engine {

	namespace scene {

		SceneNode::SceneNode():
		m_Creator(0) {
			kmMat4Identity(&m_Transform);
		}

		SceneNode::~SceneNode() {
			m_Creator = 0;
		}

		const uint SceneNode::getNumAttachedObjects() const {
			return m_Objects.size();
		}

		bool SceneNode::attachObject(shared_ptr<Entity> newObject) {
			m_Objects[newObject->getName()] = newObject;
			return true;
		}

		void SceneNode::detachObject(const string& name) {
			m_Objects.erase(name);
		}

		void SceneNode::detachAllObjects() {
			m_Objects.clear();
		}

		///Return an attached object by name
		shared_ptr<Entity> SceneNode::getAttachedObject(const string& name) {
			assert(m_Objects.find(name) != m_Objects.end());
			return m_Objects[name];
		}

		///Return the position of this node
		const Vec3 SceneNode::getPosition() const {
			return Vec3(	m_Transform.m_Mat[12],
							m_Transform.m_Mat[13],
							m_Transform.m_Mat[14] );
		}

		///Set the position of this scene node
		void SceneNode::setPosition(const Vec3& vec) {
			m_Transform.m_Mat[12] = vec.x;
			m_Transform.m_Mat[13] = vec.y;
			m_Transform.m_Mat[14] = vec.z;
		}

		/// Create a new child scene node with name and position
		/// of vec relative to the parent
		shared_ptr<SceneNode> SceneNode::createChildSceneNode(const string& name, Vec3 vec) {
			shared_ptr<SceneNode> sceneNode(new SceneNode());
			sceneNode->setPosition(vec);

			m_Children.push_back(sceneNode);

			return sceneNode;
		}

/*		shared_ptr<BSPSceneNode> SceneNode::createChildBSPSceneNode(const string& name, Vec3 vec, const shared_ptr<Q3BSP> bsp) {
			shared_ptr<BSPSceneNode> sceneNode(new BSPSceneNode(bsp));
			sceneNode->setPosition(vec);
			m_Children.push_back(sceneNode);
			return sceneNode;
		}

		shared_ptr<OctreeSceneNode> SceneNode::createChildOctreeSceneNode(const string& name, Vec3 vec, const shared_ptr<Octree> oct) {
			assert(oct);

			shared_ptr<OctreeSceneNode> sceneNode(new OctreeSceneNode(oct));
			sceneNode->setPosition(vec);
			m_Children.push_back(sceneNode);

			return sceneNode;
		}
*/
		void SceneNode::translate(const Vec3& vec) {
			m_Transform.m_Mat[12] += vec.x;
			m_Transform.m_Mat[13] += vec.y;
			m_Transform.m_Mat[14] += vec.z;
		}

		void SceneNode::scale(const Vec3& vec) {
			m_Transform.m_Mat[0] = vec.x;
			m_Transform.m_Mat[5] = vec.y;
			m_Transform.m_Mat[10] = vec.z;
		}

		void SceneNode::rotate(const Vec3& vec) {
			Mat4 x, y, z;
			kmMat4RotationX(&x, vec.x);
			kmMat4RotationY(&y, vec.y);
			kmMat4RotationZ(&z, vec.z);
			assert(0); //Continue this
		}

		const AABB& SceneNode::getBounds() const {
			return m_Bounds;
		}

		void SceneNode::updateBounds() {

		}

		void SceneNode::update(shared_ptr<Camera> activeCamera) {
			glPushMatrix();
				glMultMatrixf(m_Transform.m_Mat);
				for (map<string, shared_ptr<Entity> >::iterator it = m_Objects.begin();
					it != m_Objects.end(); ++it) {
					(*it).second->render(activeCamera);
				}

				for (vector< shared_ptr<SceneNode> > ::iterator it = m_Children.begin();
					it != m_Children.end(); ++it) {
					(*it)->update(activeCamera);
				}

			glPopMatrix();
		}
	}
}
