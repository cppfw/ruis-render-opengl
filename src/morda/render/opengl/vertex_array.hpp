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

#include <morda/render/vertex_array.hpp>

#include <GL/glew.h>

namespace morda{ namespace render_opengl{

class vertex_array : public morda::vertex_array{
public:
	const GLuint vao;

	vertex_array(
			std::vector<std::shared_ptr<morda::vertex_buffer>>&& buffers,
			std::shared_ptr<morda::index_buffer> indices,
			mode rendering_mode
		);
	
	vertex_array(const vertex_array&) = delete;
	vertex_array& operator=(const vertex_array&) = delete;

	~vertex_array();

	void bind_buffers()const;

private:

};

}}
