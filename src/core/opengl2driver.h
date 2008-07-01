#ifndef OPENGL2DRIVER_H_INCLUDED
#define OPENGL2DRIVER_H_INCLUDED

#include "utilities/colour.h"
#include "igraphicsdriver.h"

class OpenGL2Driver : public IGraphicsDriver {
	public:
		OpenGL2Driver() {
			m_Ext.vertexBufferObjects = false;
		}

		struct ExtensionsEnabled {
			bool vertexBufferObjects;
		} m_Ext;


		void beginScene();
		void endScene();
		void draw3DTriangle(const vector<Vec3>& vertices, const Colour& colour);

	protected:
		bool doInitialize();

	private:
		bool loadSupportedExtensions();
};


#endif // OPENGL2DRIVER_H_INCLUDED
