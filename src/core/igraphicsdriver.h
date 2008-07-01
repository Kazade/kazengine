#ifndef IGRAPHICSDRIVER_H_INCLUDED
#define IGRAPHICSDRIVER_H_INCLUDED

#include <kazmathxx/vec3.h>
#include <vector>
#include "utilities/colour.h"

using std::vector;

class IGraphicsDriver {
	public:
		virtual void beginScene() = 0;
		virtual void endScene() = 0;

		virtual void draw3DTriangle(const vector<Vec3>& vertices, const Colour& colour) = 0;
		virtual ~IGraphicsDriver() {}

		bool initialize() {
			bool status = doInitialize();
			return status;
		}

	protected:
		virtual bool doInitialize() = 0;
};


#endif // IGRAPHICSDRIVER_H_INCLUDED
