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

#include "shader_color_pos_tex.hpp"

#include "texture_2d.hpp"

using namespace morda::render_opengl;

shader_color_pos_tex::shader_color_pos_tex() :
		shader_base(
				R"qwertyuiop(
						#ifndef GL_ES
						#	define highp
						#	define mediump
						#	define lowp
						#endif

						attribute highp vec4 a0;

						attribute highp vec2 a1;

						uniform highp mat4 matrix;

						varying highp vec2 tc0;

						void main(void){
							gl_Position = matrix * a0;
							tc0 = a1;
						}
					)qwertyuiop",
				R"qwertyuiop(
						#ifndef GL_ES
						#	define highp
						#	define mediump
						#	define lowp
						#endif
		
						uniform sampler2D texture0;
		
						uniform highp vec4 uniform_color;
		
						varying highp vec2 tc0;
		
						void main(void){
							gl_FragColor = texture2D(texture0, tc0) * uniform_color;
						}
					)qwertyuiop"
			)
{
	this->colorUniform = this->get_uniform("uniform_color");
}

void shader_color_pos_tex::render(const r4::matrix4<float>& m, const morda::vertex_array& va, r4::vector4<float> color, const morda::texture_2d& tex)const{
	static_cast<const texture_2d&>(tex).bind(0);
	this->bind();
	
	this->set_uniform4f(this->colorUniform, color.x(), color.y(), color.z(), color.w());
	
	this->shader_base::render(m, va);
}
