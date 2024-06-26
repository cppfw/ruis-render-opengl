/*
ruis-render-opengl - OpenGL renderer

Copyright (C) 2012-2024  Ivan Gagis <igagis@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

/* ================ LICENSE END ================ */

#include "opengl_texture.hpp"

#include "util.hpp"

using namespace ruis::render::opengl;

opengl_texture::opengl_texture()
{
	glGenTextures(1, &this->tex);
	assert_opengl_no_error();
	ASSERT(this->tex != 0)
}

opengl_texture::~opengl_texture()
{
	glDeleteTextures(1, &this->tex);
}

void opengl_texture::set_active_texture(unsigned unit_num) const
{
	// OpenGL guarantees that GL_TEXTUREi = GL_TEXTURE0 + i
	glActiveTexture(GL_TEXTURE0 + unit_num);
	assert_opengl_no_error();
}

void opengl_texture::bind(unsigned unit_num) const
{
	this->set_active_texture(unit_num);

	glBindTexture(GL_TEXTURE_2D, this->tex);
	assert_opengl_no_error();
}

GLint opengl_texture::set_swizzeling(rasterimage::format f) const
{
	switch (f) {
		default:
			ASSERT(false)
		case rasterimage::format::grey:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_R, GL_RED);
			assert_opengl_no_error();
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, GL_RED);
			assert_opengl_no_error();
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, GL_RED);
			assert_opengl_no_error();

			// GL_LUMINANCE is deprecated in OpenGL 3, so we use GL_RED
			return GL_RED;
		case rasterimage::format::greya:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_R, GL_RED);
			assert_opengl_no_error();
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, GL_RED);
			assert_opengl_no_error();
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, GL_RED);
			assert_opengl_no_error();
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_A, GL_GREEN);
			assert_opengl_no_error();

			// GL_LUMINANCE_ALPHA is deprecated in OpenGL 3, so we use GL_RG
			return GL_RG;
		case rasterimage::format::rgb:
			return GL_RGB;
		case rasterimage::format::rgba:
			return GL_RGBA;
	}
}
