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

#include <ruis/render/coloring_texturing_shader.hpp>

#include "shader_base.hpp"

namespace morda::render_opengl {

class shader_color_pos_tex_alpha : public morda::coloring_texturing_shader, public shader_base
{
	GLint color_uniform;

public:
	shader_color_pos_tex_alpha();

	shader_color_pos_tex_alpha(const shader_color_pos_tex_alpha&) = delete;
	shader_color_pos_tex_alpha& operator=(const shader_color_pos_tex_alpha&) = delete;

	shader_color_pos_tex_alpha(shader_color_pos_tex_alpha&&) = delete;
	shader_color_pos_tex_alpha& operator=(shader_color_pos_tex_alpha&&) = delete;

	~shader_color_pos_tex_alpha() override = default;

	void render(
		const r4::matrix4<float>& m,
		const morda::vertex_array& va,
		r4::vector4<float> color,
		const morda::texture_2d& tex
	) const override;
};

} // namespace morda::render_opengl
