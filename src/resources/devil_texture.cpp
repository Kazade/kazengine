#include <GL/glu.h>

#include "devil_texture.h"

bool devil_texture::s_il_was_initialized = false;

file_load_status devil_texture::load(istream& stream) {
	if (!s_il_was_initialized) {
		ilInit(); //Lazy initialization
		s_il_was_initialized = true;
	}

	file_load_status result = FILE_LOAD_FAILED;

	if (load_raw_data(stream)) {
		ilGenImages(1, &m_il_texture_id);
		ilBindImage(m_il_texture_id);

		if (ilLoadL(IL_TYPE_UNKNOWN, &m_raw_data[0], m_raw_data.size())) {
			result = FILE_LOAD_SUCCESS;

			//Store the width and height of the image
			m_width = ilGetInteger(IL_IMAGE_WIDTH);
			m_height = ilGetInteger(IL_IMAGE_HEIGHT);
			m_type = ilGetInteger(IL_IMAGE_FORMAT);
			m_bpp = ilGetInteger(IL_IMAGE_BPP);
		}
	}

	return result;
}

void devil_texture::unload() {
	glDeleteTextures(1, &m_ogl_texture_id);
	ilDeleteImages(1, &m_il_texture_id);
}

bool devil_texture::load_raw_data(istream& stream) {
	//Find the length of the file
	stream.seekg(0, std::ios::end);
	std::streamsize length = stream.tellg();

	if (!length) {
		return false;
	}

	//Read the whole file into a vector
	stream.seekg(0, std::ios::beg);
	m_raw_data.resize(length);
	stream.read(&m_raw_data[0], length);

	return true;
}

GLuint devil_texture::generate(texture_generate_options options) {
	ilBindImage(m_il_texture_id);

	/* FIXME: If there is an alpha channel this should be IL_RGBA */
	ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);
	glEnable(GL_TEXTURE_2D);

	glGenTextures(1, &m_ogl_texture_id); /* Texture name generation */

  glBindTexture(GL_TEXTURE_2D, m_ogl_texture_id); /* Binding of texture name */

  //FIXME: These parameters should be read from "options"
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); /* We will use linear
      interpolation for magnification filter */
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); /* We will use linear */

	if (options && TGO_BUILD_MIPMAPS) {
		gluBuild2DMipmaps(GL_TEXTURE_2D, m_bpp, m_width, m_height,
                   m_type, GL_UNSIGNED_BYTE, ilGetData());

	} else {
		glTexImage2D(GL_TEXTURE_2D, 0, m_bpp, m_width,
      m_height, 0, m_type, GL_UNSIGNED_BYTE, ilGetData());
	}

	return m_ogl_texture_id;
}
