#ifndef MAP_TEXTURE_H
#define MAP_TEXTURE_H

#include <string>
#include <GL/gl.h>

using std::string;

typedef int resource_id;

class map_texture {
	public:
		map_texture();

		GLuint get_opengl_texture_id() { return m_opengl_texture_id; }
		void set_opengl_texture_id(GLuint id) { m_opengl_texture_id = id; }

		resource_id get_resource_id() { return m_resource_id; }
		void set_resource_id(const resource_id& id) { m_resource_id = id; }
	private:
		string m_texture_name; ///< Filename of the texture relative to a path specified when loading (e.g. textures/map1/mytex.tga)
		int m_texture_flags;
		int m_content_flags;
		GLuint m_opengl_texture_id;
		resource_id m_resource_id;
};

#endif // MAP_TEXTURE_H
