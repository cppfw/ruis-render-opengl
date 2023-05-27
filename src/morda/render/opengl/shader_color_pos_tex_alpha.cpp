#include "shader_color_pos_tex_alpha.hpp"

#include "texture_2d.hpp"

using namespace morda::render_opengl;

shader_color_pos_tex_alpha::shader_color_pos_tex_alpha() :
		shader_base(
				R"qwertyuiop(
						attribute vec4 a0;

						attribute vec2 a1;

						uniform mat4 matrix;

						varying vec2 tc0;

						void main(void){
							gl_Position = matrix * a0;
							tc0 = a1;
						}
					)qwertyuiop",
				R"qwertyuiop(		
						uniform sampler2D texture0;
		
						uniform vec4 uniform_color;
		
						varying vec2 tc0;
		
						void main(void){
							gl_FragColor = vec4(
                                uniform_color.x,
                                uniform_color.y,
                                uniform_color.z,
                                uniform_color.w * texture2D(texture0, tc0).x
                            );
						}
					)qwertyuiop"
			)
{
	this->colorUniform = this->get_uniform("uniform_color");
}

void shader_color_pos_tex_alpha::render(const r4::matrix4<float>& m, const morda::vertex_array& va, r4::vector4<float> color, const morda::texture_2d& tex)const{
	static_cast<const texture_2d&>(tex).bind(0);
	this->bind();
	
	this->set_uniform4f(this->colorUniform, color.x(), color.y(), color.z(), color.w());
	
	this->shader_base::render(m, va);
}
