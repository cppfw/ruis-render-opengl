#pragma once

#include <morda/render/texturing_shader.hpp>

#include "shader_base.hpp"

namespace morda{ namespace render_opengl{

class shader_texture :
		public morda::texturing_shader,
		public shader_base
{
	GLint textureUniform;
public:
	shader_texture();
	
	void render(const r4::matrix4<float>& m, const morda::vertex_array& va, const morda::texture_2d& tex)const override;
};

}}
