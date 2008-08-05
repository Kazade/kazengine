#include <GL/gl.h>

#include <kazmathxx/vec3.h>

#include "frustum.h"

void frustum::update_frustum() {
	float model[16] = {0};     // Array to store the modelview matrix.
	float proj[16] = {0};      // Array to store the projection matrix.

	glGetFloatv(GL_PROJECTION_MATRIX, proj);
	glGetFloatv(GL_MODELVIEW_MATRIX, model);

	calculate_frustum(model, proj);
	normalize_frustum();
}

void frustum::update_frustum(const float* model, const float* proj) {
	calculate_frustum(model, proj);
	normalize_frustum();
}

void frustum::calculate_frustum(const float* model, const float* proj) {
		// Error checking.
	if(model == 0 || proj == 0)
			return;

	// Create the clip.
	float clip[16] = {0};

	clip[0] = model[0] * proj[0] + model[1] * proj[4] + model[2] * proj[8]  + model[3] * proj[12];
	clip[1] = model[0] * proj[1] + model[1] * proj[5] + model[2] * proj[9]  + model[3] * proj[13];
	clip[2] = model[0] * proj[2] + model[1] * proj[6] + model[2] * proj[10] + model[3] * proj[14];
	clip[3] = model[0] * proj[3] + model[1] * proj[7] + model[2] * proj[11] + model[3] * proj[15];

	clip[4] = model[4] * proj[0] + model[5] * proj[4] + model[6] * proj[8]  + model[7] * proj[12];
	clip[5] = model[4] * proj[1] + model[5] * proj[5] + model[6] * proj[9]  + model[7] * proj[13];
	clip[6] = model[4] * proj[2] + model[5] * proj[6] + model[6] * proj[10] + model[7] * proj[14];
	clip[7] = model[4] * proj[3] + model[5] * proj[7] + model[6] * proj[11] + model[7] * proj[15];

	clip[8]  = model[8] * proj[0] + model[9] * proj[4] + model[10] * proj[8]  + model[11] * proj[12];
	clip[9]  = model[8] * proj[1] + model[9] * proj[5] + model[10] * proj[9]  + model[11] * proj[13];
	clip[10] = model[8] * proj[2] + model[9] * proj[6] + model[10] * proj[10] + model[11] * proj[14];
	clip[11] = model[8] * proj[3] + model[9] * proj[7] + model[10] * proj[11] + model[11] * proj[15];

	clip[12] = model[12] * proj[0] + model[13] * proj[4] + model[14] * proj[8]  + model[15] * proj[12];
	clip[13] = model[12] * proj[1] + model[13] * proj[5] + model[14] * proj[9]  + model[15] * proj[13];
	clip[14] = model[12] * proj[2] + model[13] * proj[6] + model[14] * proj[10] + model[15] * proj[14];
	clip[15] = model[12] * proj[3] + model[13] * proj[7] + model[14] * proj[11] + model[15] * proj[15];


	// Calculate the right side of the frustum.
	m_planes[0].a = clip[3]  - clip[0];
	m_planes[0].b = clip[7]  - clip[4];
	m_planes[0].c = clip[11] - clip[8];
	m_planes[0].d = clip[15] - clip[12];

	// Calculate the left side of the frustum.
	m_planes[1].a = clip[3]  + clip[0];
	m_planes[1].b = clip[7]  + clip[4];
	m_planes[1].c = clip[11] + clip[8];
	m_planes[1].d = clip[15] + clip[12];

	// Calculate the bottom side of the frustum.
	m_planes[2].a = clip[3]  + clip[1];
	m_planes[2].b = clip[7]  + clip[5];
	m_planes[2].c = clip[11] + clip[9];
	m_planes[2].d = clip[15] + clip[13];

	// Calculate the top side of the frustum.
	m_planes[3].a = clip[3]  - clip[1];
	m_planes[3].b = clip[7]  - clip[5];
	m_planes[3].c = clip[11] - clip[9];
	m_planes[3].d = clip[15] - clip[13];

	// Calculate the far side of the frustum.
	m_planes[4].a = clip[3]  - clip[2];
	m_planes[4].b = clip[7]  - clip[6];
	m_planes[4].c = clip[11] - clip[10];
	m_planes[4].d = clip[15] - clip[14];

	// Calculate the near side of the frustum.
	m_planes[5].a = clip[3]  + clip[2];
	m_planes[5].b = clip[7]  + clip[6];
	m_planes[5].c = clip[11] + clip[10];
	m_planes[5].d = clip[15] + clip[14];
}

void frustum::normalize_frustum() {
	float magnitude = 0.0f;

	// Loop through each side of the frustum and normalize it.
	for(int i = 0; i < 6; i++)
	{
			magnitude = (float)sqrt(m_planes[i].a * m_planes[i].a +
														 m_planes[i].b * m_planes[i].b +
																 m_planes[i].c * m_planes[i].c);
			magnitude = 1.0f / magnitude;

			m_planes[i].a *= magnitude;
			m_planes[i].b *= magnitude;
			m_planes[i].c *= magnitude;
			m_planes[i].d *= magnitude;
	}
}

bool frustum::cube_in_frustum(const float* p, const float size) {
	float minX, maxX;
	float minY, maxY;
	float minZ, maxZ;

	float x = p[0];
	float y = p[1];
	float z = p[2];

	// Calculate the bounding box.
	minX = x - size;
	maxX = x + size;

	minY = y - size;
	maxY = y + size;

	minZ = z - size;
	maxZ = z + size;

	// Loop through each side of the frustum and test if the box lies outside any of them.
	for(int i = 0; i < 6; i++)
	{
			if(m_planes[i].getDistance(Vec3(minX, minY, minZ)) > 0) continue;
			if(m_planes[i].getDistance(Vec3(maxX, minY, minZ)) > 0) continue;
			if(m_planes[i].getDistance(Vec3(minX, maxY, minZ)) > 0) continue;
			if(m_planes[i].getDistance(Vec3(maxX, maxY, minZ)) > 0) continue;
			if(m_planes[i].getDistance(Vec3(minX, minY, maxZ)) > 0) continue;
			if(m_planes[i].getDistance(Vec3(maxX, minY, maxZ)) > 0) continue;
			if(m_planes[i].getDistance(Vec3(minX, maxY, maxZ)) > 0) continue;
			if(m_planes[i].getDistance(Vec3(maxX, maxY, maxZ)) > 0) continue;

			return false;
	}

	return true;
}

bool frustum::aabb_in_frustum(const kmAABB& aabb) {
	//TODO: WRITE THIS CODE
	return true;
}
