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

#pragma once

#include <GL/glew.h>
#include <ruis/render/vertex_array.hpp>

namespace ruis::render::opengl {

class vertex_array : public ruis::render::vertex_array
{
public:
	// NOTE: The native OpenGL Vertex Array Object is not used because it cannot be shared between OpenGL contexts.

	vertex_array(
		utki::shared_ref<ruis::render::renderer> renderer, //
		buffers_type buffers,
		utki::shared_ref<const ruis::render::index_buffer> indices,
		mode rendering_mode
	);

	vertex_array(const vertex_array&) = delete;
	vertex_array& operator=(const vertex_array&) = delete;

	vertex_array(vertex_array&&) = delete;
	vertex_array& operator=(vertex_array&&) = delete;

	~vertex_array() override = default;

	void bind_buffers() const;

private:
};

} // namespace ruis::render::opengl
