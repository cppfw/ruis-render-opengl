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

#include <GL/glew.h>

namespace morda{ namespace render_opengl{

class opengl_buffer{
public:
	const GLuint buffer;

	opengl_buffer();
	
	opengl_buffer(const opengl_buffer&) = delete;
	opengl_buffer& operator=(const opengl_buffer&) = delete;
	
	virtual ~opengl_buffer()noexcept;
	
private:

};

}}
