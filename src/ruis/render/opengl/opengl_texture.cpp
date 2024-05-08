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

void opengl_texture::bind(unsigned unit_num) const
{
	glActiveTexture(GL_TEXTURE0 + unit_num);
	assert_opengl_no_error();
	glBindTexture(GL_TEXTURE_2D, this->tex);
	assert_opengl_no_error();
}
