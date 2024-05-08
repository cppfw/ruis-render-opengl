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

#include "vertex_array.hpp"

#include "index_buffer.hpp"
#include "util.hpp"
#include "vertex_buffer.hpp"

using namespace ruis::render::opengl;

vertex_array::vertex_array(
	buffers_type buffers,
	const utki::shared_ref<const ruis::render::index_buffer>& indices,
	mode rendering_mode
) :
	ruis::render::vertex_array(std::move(buffers), indices, rendering_mode),
	vao([]() {
		if (GLEW_ARB_vertex_array_object) {
			// NOLINTNEXTLINE(cppcoreguidelines-init-variables)
			GLuint ret;
			glGenVertexArrays(1, &ret);
			assert_opengl_no_error();
			return ret;
		} else {
			return GLuint(0);
		}
	}())
{
	if (GLEW_ARB_vertex_array_object) {
		glBindVertexArray(this->vao);
		assert_opengl_no_error();

		this->bind_buffers();

		glBindVertexArray(0);
		assert_opengl_no_error();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		assert_opengl_no_error();
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		assert_opengl_no_error();
	}
}

vertex_array::~vertex_array()
{
	if (GLEW_ARB_vertex_array_object) {
		glBindVertexArray(0);
		assert_opengl_no_error();
		glDeleteVertexArrays(1, &this->vao);
		assert_opengl_no_error();
	}
}

void vertex_array::bind_buffers() const
{
	for (unsigned i = 0; i != this->buffers.size(); ++i) {
		ASSERT(dynamic_cast<const vertex_buffer*>(&this->buffers[i].get()))
		// NOLINTNEXTLINE(cppcoreguidelines-pro-type-static-cast-downcast)
		const auto& vbo = static_cast<const vertex_buffer&>(this->buffers[i].get());
		glBindBuffer(GL_ARRAY_BUFFER, vbo.buffer);
		assert_opengl_no_error();

		//		TRACE(<< "vbo.numComponents = " << vbo.numComponents << "
		// vbo.type = " << vbo.type << std::endl)

		glVertexAttribPointer(i, vbo.num_components, vbo.type, GL_FALSE, 0, nullptr);
		assert_opengl_no_error();

		glEnableVertexAttribArray(i);
		assert_opengl_no_error();
	}

	{
		ASSERT(dynamic_cast<const index_buffer*>(&this->indices.get()))
		// NOLINTNEXTLINE(cppcoreguidelines-pro-type-static-cast-downcast)
		const auto& ivbo = static_cast<const index_buffer&>(this->indices.get());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ivbo.buffer);
		assert_opengl_no_error();
	}
}
