/*
morda-render-opengl - OpenGL GUI renderer

Copyright (C) 2012-2023  Ivan Gagis <igagis@gmail.com>

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

#include "opengl_buffer.hpp"

#include "util.hpp"

using namespace morda::render_opengl;

opengl_buffer::opengl_buffer() :
	buffer([]() -> GLuint {
		// NOLINTNEXTLINE(cppcoreguidelines-init-variables)
		GLuint ret;
		glGenBuffers(1, &ret);
		assert_opengl_no_error();
		return ret;
	}())
{}

opengl_buffer::~opengl_buffer()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	assert_opengl_no_error();
	glDeleteBuffers(1, &this->buffer);
	assert_opengl_no_error();
}
