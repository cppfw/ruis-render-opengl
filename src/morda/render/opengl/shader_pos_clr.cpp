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

#include "shader_pos_clr.hpp"

using namespace morda::render_opengl;

shader_pos_clr::shader_pos_clr() :
	shader_base(
		R"qwertyuiop(
						uniform mat4 matrix;

						attribute vec4 a0;
						attribute vec4 a1;

						varying vec4 color_varying;

						void main(void){
							gl_Position = matrix * a0;
							color_varying = a1;
						}
					)qwertyuiop",
		R"qwertyuiop(
						varying vec4 color_varying;
						
						void main(void){
							gl_FragColor = color_varying;
						}
					)qwertyuiop"
	)
{}

void shader_pos_clr::render(const r4::matrix4<float>& m, const morda::vertex_array& va) const
{
	this->bind();

	this->shader_base::render(m, va);
}
