#include <GL/gl.h>
#include <boost/filesystem.hpp>

#include "scene/skybox.h"

using boost::filesystem::path;

namespace engine {

	namespace scene {

		Skybox::Skybox():
		Entity("sky") {}

		Skybox::~Skybox() {

		}

		bool Skybox::loadSkybox(const string& filepath) {
			string top = "top.tga";
			string bottom = "bottom.tga";
			string left = "left.tga";
			string right = "right.tga";
			string front = "front.tga";
			string back = "back.tga";
			/*
			string top = (path(filepath) / "top.tga").string();
			string bottom = (path(filepath) / "bottom.tga").string();
			string left = (path(filepath) / "left.tga").string();
			string right = (path(filepath) / "right.tga").string();
			string front = (path(filepath) / "front.tga").string();
			string back = (path(filepath) / "back.tga").string();*/

			getTextureManager()->add_to_search_path(filepath);

/*			if (!getTextureManager()->loadResource(top, "SKY_TOP")) {
				return false;
			}

			if (!getTextureManager()->loadResource(bottom, "SKY_BOTTOM")) {
				return false;
			}

			if (!getTextureManager()->loadResource(left, "SKY_LEFT")) {
				return false;
			}

			if (!getTextureManager()->loadResource(right, "SKY_RIGHT")) {
				return false;
			}

			if (!getTextureManager()->loadResource(front, "SKY_FRONT")) {
				return false;
			}

			if (!getTextureManager()->loadResource(back, "SKY_BACK")) {
				return false;
			}*/

/*			getTextureManager()->getResourceByName("SKY_TOP")->generateTexture(false);
			getTextureManager()->getResourceByName("SKY_BOTTOM")->generateTexture(false);
			getTextureManager()->getResourceByName("SKY_LEFT")->generateTexture(false);
			getTextureManager()->getResourceByName("SKY_RIGHT")->generateTexture(false);
			getTextureManager()->getResourceByName("SKY_FRONT")->generateTexture(false);
			getTextureManager()->getResourceByName("SKY_BACK")->generateTexture(false);*/

			return true;
		}

		void Skybox::render(shared_ptr<Camera> activeCamera) {
            float model[16];
            glGetFloatv(GL_MODELVIEW_MATRIX, model);
		    glPushMatrix();
		    glLoadIdentity();
		    model[12] = model[13] = model[14] = 0.0f;
            glMultMatrixf(model);

			glDisable(GL_DEPTH_TEST);
			//glDepthMask(GL_FALSE);
			glColor3f(1, 1, 1);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glEnable(GL_TEXTURE_2D);

//			glBindTexture(GL_TEXTURE_2D, getTextureManager()->getResourceByName("SKY_FRONT")->getID());  // Bind the Texture to the object

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glBegin(GL_QUADS);
			glTexCoord2f(0.0, 0.0); glVertex3f(-SKYBOX_SIZE, -SKYBOX_SIZE,  SKYBOX_SIZE);
			glTexCoord2f(1.0, 0.0); glVertex3f( SKYBOX_SIZE, -SKYBOX_SIZE,  SKYBOX_SIZE);
			glTexCoord2f(1.0, 1.0); glVertex3f( SKYBOX_SIZE,  SKYBOX_SIZE,  SKYBOX_SIZE);
			glTexCoord2f(0.0, 1.0); glVertex3f(-SKYBOX_SIZE,  SKYBOX_SIZE,  SKYBOX_SIZE);
			glEnd();

//			glBindTexture(GL_TEXTURE_2D, getTextureManager()->getResourceByName("SKY_BACK")->getID());
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glBegin(GL_QUADS);
			glNormal3f( 0.0, 0.0,-1.0);
			glTexCoord2f(1.0, 0.0); glVertex3f(-SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE);
			glTexCoord2f(1.0, 1.0); glVertex3f(-SKYBOX_SIZE,  SKYBOX_SIZE, -SKYBOX_SIZE);
			glTexCoord2f(0.0, 1.0); glVertex3f( SKYBOX_SIZE,  SKYBOX_SIZE, -SKYBOX_SIZE);
			glTexCoord2f(0.0, 0.0); glVertex3f( SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE);
			glEnd();

	//		glBindTexture(GL_TEXTURE_2D, getTextureManager()->getResourceByName("SKY_TOP")->getID());
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glBegin(GL_QUADS);
			glNormal3f( 0.0, 1.0, 0.0);
			glTexCoord2f(0.0, 1.0); glVertex3f(-SKYBOX_SIZE,  SKYBOX_SIZE, -SKYBOX_SIZE);
			glTexCoord2f(0.0, 0.0); glVertex3f(-SKYBOX_SIZE,  SKYBOX_SIZE,  SKYBOX_SIZE);
			glTexCoord2f(1.0, 0.0); glVertex3f( SKYBOX_SIZE,  SKYBOX_SIZE,  SKYBOX_SIZE);
			glTexCoord2f(1.0, 1.0); glVertex3f( SKYBOX_SIZE,  SKYBOX_SIZE, -SKYBOX_SIZE);
			glEnd();

	//		glBindTexture(GL_TEXTURE_2D, getTextureManager()->getResourceByName("SKY_BOTTOM")->getID());
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glBegin(GL_QUADS);
			glNormal3f( 0.0,-1.0, 0.0);
			glTexCoord2f(0.0, 0.0); glVertex3f(-SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE);
			glTexCoord2f(1.0, 0.0); glVertex3f( SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE);
			glTexCoord2f(1.0, 1.0); glVertex3f( SKYBOX_SIZE, -SKYBOX_SIZE,  SKYBOX_SIZE);
			glTexCoord2f(0.0, 1.0); glVertex3f(-SKYBOX_SIZE, -SKYBOX_SIZE,  SKYBOX_SIZE);
			glEnd();

	//		glBindTexture(GL_TEXTURE_2D, getTextureManager()->getResourceByName("SKY_LEFT")->getID());
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glBegin(GL_QUADS);
			glNormal3f( 1.0, 0.0, 0.0);
			glTexCoord2f(1.0, 0.0); glVertex3f( SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE);
			glTexCoord2f(1.0, 1.0); glVertex3f( SKYBOX_SIZE,  SKYBOX_SIZE, -SKYBOX_SIZE);
			glTexCoord2f(0.0, 1.0); glVertex3f( SKYBOX_SIZE,  SKYBOX_SIZE,  SKYBOX_SIZE);
			glTexCoord2f(0.0, 0.0); glVertex3f( SKYBOX_SIZE, -SKYBOX_SIZE,  SKYBOX_SIZE);
			glEnd();

	//		glBindTexture(GL_TEXTURE_2D, getTextureManager()->getResourceByName("SKY_RIGHT")->getID());
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glBegin(GL_QUADS);
			glNormal3f(-1.0, 0.0, 0.0);
			glTexCoord2f(0.0, 0.0); glVertex3f(-SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE);
			glTexCoord2f(1.0, 0.0); glVertex3f(-SKYBOX_SIZE, -SKYBOX_SIZE,  SKYBOX_SIZE);
			glTexCoord2f(1.0, 1.0); glVertex3f(-SKYBOX_SIZE,  SKYBOX_SIZE,  SKYBOX_SIZE);
			glTexCoord2f(0.0, 1.0); glVertex3f(-SKYBOX_SIZE,  SKYBOX_SIZE, -SKYBOX_SIZE);

			glEnd();

			glEnable(GL_DEPTH_TEST);
			//glDepthMask(GL_TRUE);
			glPopMatrix();
		}
	}
}
