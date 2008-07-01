
#include <kazmath/utility.h>
#include <kazmathxx/mat4.h>
#include <kazmathxx/vec3.h>
#include <GL/gl.h>

#include "scene/camera.h"

namespace engine {

	namespace scene {

		Camera::Camera(const string& name):
		yawFixed(false),
		m_FrustumNeedsUpdating(true) {
			m_Name = name;
			kmQuaternionIdentity(&m_Orientation);
		}

		Camera::Camera(const string& name, const Vec3& vec):
		yawFixed(false),
		m_FrustumNeedsUpdating(true) {
			m_Name = name;
			m_Position = vec;
			kmQuaternionIdentity(&m_Orientation);
		}

		Camera::Camera(const string& name, float x, float y, float z):
		yawFixed(false),
		m_FrustumNeedsUpdating(true) {
			m_Name = name;
			m_Position = Vec3(x, y, z);
			kmQuaternionIdentity(&m_Orientation);
		}

		void Camera::setPosition(const Vec3& vec) {
			m_Position = vec;
		}

		void Camera::lookAt(const Vec3& vec) {
			Vec3 newForward;
			newForward = vec - m_Position;
			setDirection(newForward);
			m_FrustumNeedsUpdating = true;
		}

		void Camera::setNearClipDistance(float dist) {

		}

		void Camera::setFarClipDistance(float dist) {

		}

		void Camera::yaw(float angle) {
			Vec3 yAxis;

			if (yawFixed) {
				// Rotate around fixed yaw axis
				yAxis = yawFixedAxis;
			} else {
				Vec3 UNIT_Y(0, 1, 0);
				kmQuaternionMultiplyVec3(&yAxis, &m_Orientation, &UNIT_Y);
			}

			rotate(yAxis, angle);
			m_FrustumNeedsUpdating = true;
		}

		void Camera::pitch(float angle) {
			Vec3 xAxis;

			Vec3 UNIT_X(1, 0, 0);
			kmQuaternionMultiplyVec3(&xAxis, &m_Orientation, &UNIT_X);

			rotate(xAxis, angle);

			m_FrustumNeedsUpdating = true;
		}

		void Camera::roll(float val) {

		}

		void Camera::rotate(const Vec3& axis, float angle) {
			kmQuaternion q;
			kmQuaternionRotationAxis(&q, &axis, kmDegreesToRadians(angle));

			rotate(q);
		}

		void Camera::rotate(const kmQuaternion& q) {
		    kmQuaternion q2;

		    kmQuaternionNormalize(&q2, &q);
			kmQuaternionMultiply(&m_Orientation, &q, &m_Orientation);
			kmQuaternionNormalize(&m_Orientation, &m_Orientation);
			m_FrustumNeedsUpdating = true;
		}

/*		Vec3 Camera::getUp() const {
			return Vec3();
		}

		Vec3 Camera::getRight() const {
			return Vec3();
		}

		Vec3 Camera::getForward() const {
			return Vec3();
		}*/

		string Camera::getName() {
			return m_Name;
		}

		void Camera::setFixedYawAxis(bool fixYaw, const Vec3& axis) {
			yawFixed = fixYaw;
			yawFixedAxis = axis;
		}

		void Camera::setOrientation(const kmQuaternion& q) {
			m_Orientation = q;
		}

		void Camera::setDirection(const Vec3& vec) {
			//Do nothing if the vector passed has no length
			if (vec==Vec3()) {
				return;
			}

			//Get the normalized vector, negated to take into accoun the Z axis

			Vec3 normDir(-vec);
			kmVec3Normalize(&normDir, &normDir);

			//If the yaw axis is fixed (i.e. first person camera)
			if (yawFixed) {
				Vec3 right;
				kmVec3Cross(&right, &yawFixedAxis, &normDir);

				Vec3 up;
				kmVec3Cross(&up, &normDir, &right);

				//Set the orientation
				kmMat4 rotation;
				kmMat4Identity(&rotation);

				rotation.m_Mat[0] = right.x;
				rotation.m_Mat[1] = right.y;
				rotation.m_Mat[2] = right.z;

				rotation.m_Mat[4] = up.x;
				rotation.m_Mat[5] = up.y;
				rotation.m_Mat[6] = up.z;

				rotation.m_Mat[8] = normDir.x;
				rotation.m_Mat[9] = normDir.y;
				rotation.m_Mat[10] = normDir.z;

				kmQuaternionRotationMatrix(&m_Orientation, &rotation);
			} else {
				Vec3 axes[3];
				Mat4 rotation;

				kmMat4RotationQuaternion(&rotation, &m_Orientation);
				axes[0].x = rotation.m_Mat[0];
				axes[0].y = rotation.m_Mat[1];
				axes[0].z = rotation.m_Mat[2];

				axes[1].x = rotation.m_Mat[4];
				axes[1].y = rotation.m_Mat[5];
				axes[1].z = rotation.m_Mat[6];

				axes[2].x = rotation.m_Mat[8];
				axes[2].y = rotation.m_Mat[9];
				axes[2].z = rotation.m_Mat[10];


				kmQuaternion rotQuat;
				Vec3 temp = axes[2] + normDir;
				if (kmVec3LengthSq(&temp) < 0.00005f) {
					kmQuaternionRotationAxis(&rotQuat, &axes[1], kmPI);
				} else {
					Vec3 fallback;
					kmQuaternionRotationBetweenVec3(&rotQuat, &axes[2], &normDir, &fallback);
				}

				kmQuaternionMultiply(&m_Orientation, &rotQuat, &m_Orientation);
				kmQuaternionNormalize(&m_Orientation, &m_Orientation);

			}
			m_FrustumNeedsUpdating = true;
		}

		void Camera::use() {
			kmMat4 transform;

			kmMat4RotationQuaternion(&transform, &m_Orientation);
            glMultMatrixf(transform.m_Mat);

			transform.m_Mat[12] = m_Position.x;
			transform.m_Mat[13] = m_Position.y;
			transform.m_Mat[14] = m_Position.z;

            glTranslatef(m_Position.x, m_Position.y, m_Position.z);

		    if (m_FrustumNeedsUpdating) {
	            float model[16] = {0};     // Array to store the modelview matrix.
                float proj[16] = {0};      // Array to store the projection matrix.

                glGetFloatv(GL_PROJECTION_MATRIX, proj);
                glGetFloatv(GL_MODELVIEW_MATRIX, model);
                m_Frustum.update_frustum(model, proj);
		    }
		}

	    void Camera::moveRelative(const Vec3& vec) {
            // Transform the axes of the relative vector by camera's local axes
            Vec3 trans;

            kmQuaternionMultiplyVec3(&trans, &m_Orientation, &vec);
            m_Position = m_Position + trans;
            m_FrustumNeedsUpdating = true;
        }
	}
}
