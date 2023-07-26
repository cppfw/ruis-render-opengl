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

#include "index_buffer.hpp"

#include <GL/glew.h>

#include "util.hpp"

using namespace morda::render_opengl;

index_buffer::index_buffer(utki::span<const uint16_t> indices) :
	elements_count(GLsizei(indices.size()))
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->buffer);
	assert_opengl_no_error();

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, GLsizeiptr(indices.size_bytes()), indices.data(), GL_STATIC_DRAW);
	assert_opengl_no_error();
}
