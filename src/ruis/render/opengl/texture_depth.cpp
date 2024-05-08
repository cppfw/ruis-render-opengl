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

#include "texture_depth.hpp"

#include "util.hpp"

using namespace ruis::render::opengl;

texture_depth::texture_depth(r4::vector2<uint32_t> dims) :
	ruis::render::texture_depth(dims)
{
	this->bind(0);

	glTexImage2D( //
		GL_TEXTURE_2D,
		0, // 0th level, no mipmaps
		GL_DEPTH_COMPONENT, // TODO: GL_DEPTH_COMPONENT16? // internal format
		GLsizei(dims.x()),
		GLsizei(dims.y()),
		0, // border, deprecated, should be 0
		GL_DEPTH_COMPONENT, // format of the texel data
		GL_FLOAT, // TODO: GL_UNSIGNED_SHORT? // data type of the texel data
		nullptr // texel data
	);
	assert_opengl_no_error();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	assert_opengl_no_error();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	assert_opengl_no_error();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	assert_opengl_no_error();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	assert_opengl_no_error();
}