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

#include "vertex_buffer.hpp"

#include "util.hpp"

using namespace morda::render_opengl;

void vertex_buffer::init(GLsizeiptr size, const GLvoid* data){
	glBindBuffer(GL_ARRAY_BUFFER, this->buffer);
	assert_opengl_no_error();
	
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	assert_opengl_no_error();
}

vertex_buffer::vertex_buffer(utki::span<const r4::vector4<float>> vertices) :
		morda::vertex_buffer(vertices.size()),
		num_components(4),
		type(GL_FLOAT)
{
	this->init(vertices.size_bytes(), vertices.data());
}

vertex_buffer::vertex_buffer(utki::span<const r4::vector3<float>> vertices) :
		morda::vertex_buffer(vertices.size()),
		num_components(3),
		type(GL_FLOAT)
{
	this->init(vertices.size_bytes(), vertices.data());
}

vertex_buffer::vertex_buffer(utki::span<const r4::vector2<float>> vertices) :
		morda::vertex_buffer(vertices.size()),
		num_components(2),
		type(GL_FLOAT)
{
	this->init(vertices.size_bytes(), vertices.data());
}

vertex_buffer::vertex_buffer(utki::span<const float> vertices) :
		morda::vertex_buffer(vertices.size()),
		num_components(1),
		type(GL_FLOAT)
{
	this->init(vertices.size_bytes(), vertices.data());
}
