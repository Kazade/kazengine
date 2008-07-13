#ifndef DEVIL_TEXTURE_H_INCLUDED
#define DEVIL_TEXTURE_H_INCLUDED

#include <cassert>
#include <IL/il.h>
#include <vector>

#include "iresource.h"
#include "itexture.h"

using std::vector;

class devil_texture : public resource_interface, public texture_interface {
	public:
		virtual file_load_status load(istream& stream);

		virtual void unload();

		virtual const byte* get_data() const {
			assert(!m_raw_data.empty() && "The texture data has not been loaded");
			return &m_raw_data[0];
		}

		virtual int get_width() const {
			return m_width;
		}

		virtual int get_height() const {
			return m_height;
		}

		virtual GLenum get_type() const {
			return GLenum(m_type);
		}

		virtual GLuint generate(texture_generate_options options=TGO_BUILD_MIPMAPS);

		virtual GLuint get_ogl_texture_id() const {
			return m_ogl_texture_id;
		}

	private:
		ILuint m_il_texture_id;

		bool load_raw_data(istream& stream);
		vector<byte> m_raw_data;

		ILint m_type;
		ILint m_bpp;
		int m_width;
		int m_height;

		GLuint m_ogl_texture_id;
};

#endif // DEVIL_TEXTURE_H_INCLUDED
