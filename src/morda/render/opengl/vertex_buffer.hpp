/*
morda-render-opengl - OpenGL GUI renderer

Copyright (C) 2012-2021  Ivan Gagis <igagis@gmail.com>

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

#pragma once

#include <utki/span.hpp>

#include <r4/vector.hpp>

#include <morda/render/vertex_buffer.hpp>

#include "opengl_buffer.hpp"

namespace morda{ namespace render_opengl{

class vertex_buffer :
		public morda::vertex_buffer,
		public opengl_buffer
{
public:
	const GLint num_components;
	const GLenum type;
	
	vertex_buffer(utki::span<const r4::vector4<float>> vertices);
	
	vertex_buffer(utki::span<const r4::vector3<float>> vertices);
	
	vertex_buffer(utki::span<const r4::vector2<float>> vertices);
	
	vertex_buffer(utki::span<const float> vertices);
	
	vertex_buffer(const vertex_buffer&) = delete;
	vertex_buffer& operator=(const vertex_buffer&) = delete;

private:
	void init(GLsizeiptr size, const GLvoid* data);
};

}}
