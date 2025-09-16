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
	utki::assert(this->tex != 0, SL);
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

GLint opengl_texture::set_swizzeling(
	rasterimage::format f, //
	const utki::flags<extension>& supported_extensions
) const
{
	switch (f) {
		default:
			utki::assert(false, SL);
		case rasterimage::format::grey:
			if (supported_extensions.get(extension::ext_texture_swizzle)) {
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_R, GL_RED);
				assert_opengl_no_error();
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, GL_RED);
				assert_opengl_no_error();
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, GL_RED);
				assert_opengl_no_error();
				return GL_RED;
			} else {
				// swizzling is not supported, so we have to use GL_LUMINANCE which is deprecated in OpenGL 3
				return GL_LUMINANCE;
			}
		case rasterimage::format::greya:
			if (supported_extensions.get(extension::ext_texture_swizzle)) {
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_R, GL_RED);
				assert_opengl_no_error();
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, GL_RED);
				assert_opengl_no_error();
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, GL_RED);
				assert_opengl_no_error();
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_A, GL_GREEN);
				assert_opengl_no_error();
				return GL_RG;
			} else {
				// swizzling is not supported, so we have to use GL_LUMINANCE_ALPHA which is deprecated in OpenGL 3
				return GL_LUMINANCE_ALPHA;
			}
		case rasterimage::format::rgb:
			return GL_RGB;
		case rasterimage::format::rgba:
			return GL_RGBA;
	}
}
