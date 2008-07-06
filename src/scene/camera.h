#ifndef CAMERA_H_INCLUDED
#define CAMERA_H_INCLUDED

#include <kazmathxx/vec3.h>
#include <kazmath/quaternion.h>
#include <string>

#include "scene/frustum.h"

using std::string;

class Camera {
	private:
		Vec3 			m_Position;
		kmQuaternion 	m_Orientation;

		string 			m_Name;

		Vec3			yawFixedAxis;
		bool 			yawFixed;
		bool 			m_FrustumNeedsUpdating;

		frustum m_Frustum;

	public:
		Camera(const string& name);
		Camera(const string& name, const Vec3& vec);
		Camera(const string& name, float x, float y, float z);

		string getName();

		void setPosition(const Vec3& vec);
		const Vec3& getPosition() { return m_Position; }
		void lookAt(const Vec3& vec);
		void setNearClipDistance(float dist);
		void setFarClipDistance(float dist);

		void yaw(float val);
		void pitch(float val);
		void roll(float val);

		void rotate(const Vec3& axis, float angle);
		void rotate(const kmQuaternion& q);

		Vec3 getUp() const;
		Vec3 getForward() const;

		void setOrientation(const kmQuaternion& q);
		void setDirection(const Vec3& vec);
		void setFixedYawAxis(bool fixYaw, const Vec3& axis);
		void use();

		const frustum& getFrustum() { return m_Frustum; }
		void moveRelative(const Vec3& vec);
};

#endif // CAMERA_H_INCLUDED
