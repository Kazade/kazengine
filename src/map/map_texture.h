#ifndef MAP_TEXTURE_H
#define MAP_TEXTURE_H

#include <string>
#include <GL/gl.h>

using std::string;

class map_texture {
	public:
		map_texture();

		GLuint get_opengl_texture_id() { return m_opengl_texture_id; }

	private:
		string m_texture_name; ///< Filename of the texture relative to a path specified when loading (e.g. textures/map1/mytex.tga)
		int m_texture_flags;
		int m_content_flags;
		GLuint m_opengl_texture_id;
};

#endif // MAP_TEXTURE_H
