#ifndef ITEXTURE_H_INCLUDED
#define ITEXTURE_H_INCLUDED

#include <GL/gl.h>

typedef char byte;

enum texture_generate_options {
	TGO_BUILD_MIPMAPS = 0x01
};

class texture_interface {
	public:
		virtual ~texture_interface() {}

		virtual const byte* get_data() const = 0;
		virtual int get_width() const = 0;
		virtual int get_height() const = 0;

		virtual GLenum get_type() const = 0;
		virtual GLuint generate(texture_generate_options options=TGO_BUILD_MIPMAPS) = 0;

		virtual GLuint get_ogl_texture_id() const = 0;
};

#endif // ITEXTURE_H_INCLUDED
