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

#include "index_buffer.hpp"

#include <GL/glew.h>

#include "util.hpp"

using namespace ruis::render::opengl;

index_buffer::index_buffer(const void* data, size_t size_bytes, size_t size, GLenum element_type) :
	element_type(element_type),
	elements_count(GLsizei(size))
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->buffer);
	assert_opengl_no_error();

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, GLsizeiptr(size_bytes), data, GL_STATIC_DRAW);
	assert_opengl_no_error();
}

index_buffer::index_buffer(utki::span<const uint16_t> indices) :
	index_buffer(indices.data(), indices.size_bytes(), indices.size(), GL_UNSIGNED_SHORT)
{}

index_buffer::index_buffer(utki::span<const uint32_t> indices) :
	index_buffer(indices.data(), indices.size_bytes(), indices.size(), GL_UNSIGNED_INT)
{}
