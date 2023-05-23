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

#pragma once

#include <morda/render/texture_2d.hpp>

#include <GL/glew.h>

namespace morda{ namespace render_opengl{

struct texture_2d : public morda::texture_2d{
	GLuint tex;
	
	texture_2d(r4::vector2<float> dims);
	
	~texture_2d()noexcept;
	
	void bind(unsigned unitNum)const;
};

}}
