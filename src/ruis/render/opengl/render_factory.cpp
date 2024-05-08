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

#include "render_factory.hpp"

#include <GL/glew.h>

#include "shaders/shader_color.hpp"
#include "shaders/shader_color_pos_lum.hpp"
#include "shaders/shader_color_pos_tex.hpp"
#include "shaders/shader_color_pos_tex_alpha.hpp"
#include "shaders/shader_pos_clr.hpp"
#include "shaders/shader_pos_tex.hpp"

#include "frame_buffer.hpp"
#include "index_buffer.hpp"
#include "texture_2d.hpp"
#include "texture_depth.hpp"
#include "util.hpp"
#include "vertex_array.hpp"
#include "vertex_buffer.hpp"

using namespace ruis::render::opengl;

render_factory::render_factory()
{
	// check that the OpenGL version we have supports shaders
	if (!GLEW_ARB_vertex_shader || !GLEW_ARB_fragment_shader) {
		std::stringstream ss;
		ss << "OpenGL version '" << glGetString(GL_VERSION) << "' does not support shaders";
		throw std::runtime_error(ss.str());
	}
}

utki::shared_ref<ruis::render::texture_2d> render_factory::create_texture_2d(
	rasterimage::format format,
	rasterimage::dimensioned::dimensions_type dims,
	texture_2d_parameters params
)
{
	return this->create_texture_2d_internal(format, dims, nullptr, std::move(params));
}

utki::shared_ref<ruis::render::texture_2d> render_factory::create_texture_2d(
	const rasterimage::image_variant& imvar,
	texture_2d_parameters params
)
{
	auto imvar_copy = imvar;
	return this->create_texture_2d(std::move(imvar_copy), std::move(params));
}

utki::shared_ref<ruis::render::texture_2d> render_factory::create_texture_2d(
	rasterimage::image_variant&& imvar,
	texture_2d_parameters params
)
{
	auto iv = std::move(imvar);
	return std::visit(
		[this, &imvar = iv, &params](auto&& im) -> utki::shared_ref<ruis::render::texture_2d> {
			if constexpr (sizeof(im.pixels().front().front()) != 1) {
				throw std::logic_error(
					"render_factory::create_texture_2d(): "
					"non-8bit images are not supported"
				);
			} else {
				im.span().flip_vertical();
				auto data = im.pixels();
				return this->create_texture_2d_internal(
					imvar.get_format(),
					im.dims(),
					utki::make_span(data.front().data(), data.size_bytes()),
					std::move(params)
				);
			}
		},
		iv.variant
	);
}

utki::shared_ref<ruis::render::texture_2d> render_factory::create_texture_2d_internal(
	rasterimage::format type,
	rasterimage::dimensioned::dimensions_type dims,
	utki::span<const uint8_t> data,
	texture_2d_parameters params
)
{
	return utki::make_shared<texture_2d>(
		type, //
		dims,
		data,
		params
	);
}

utki::shared_ref<ruis::render::texture_depth> render_factory::create_texture_depth(
	rasterimage::dimensioned::dimensions_type dims
)
{
	return utki::make_shared<texture_depth>(dims);
}

utki::shared_ref<ruis::render::vertex_buffer> render_factory::create_vertex_buffer(
	utki::span<const r4::vector4<float>> vertices
)
{
	return utki::make_shared<vertex_buffer>(vertices);
}

utki::shared_ref<ruis::render::vertex_buffer> render_factory::create_vertex_buffer(
	utki::span<const r4::vector3<float>> vertices
)
{
	return utki::make_shared<vertex_buffer>(vertices);
}

utki::shared_ref<ruis::render::vertex_buffer> render_factory::create_vertex_buffer(
	utki::span<const r4::vector2<float>> vertices
)
{
	return utki::make_shared<vertex_buffer>(vertices);
}

utki::shared_ref<ruis::render::vertex_buffer> render_factory::create_vertex_buffer(utki::span<const float> vertices)
{
	return utki::make_shared<vertex_buffer>(vertices);
}

utki::shared_ref<ruis::render::vertex_array> render_factory::create_vertex_array(
	std::vector<utki::shared_ref<const ruis::render::vertex_buffer>> buffers,
	const utki::shared_ref<const ruis::render::index_buffer>& indices,
	ruis::render::vertex_array::mode mode
)
{
	return utki::make_shared<vertex_array>(std::move(buffers), indices, mode);
}

utki::shared_ref<ruis::render::index_buffer> render_factory::create_index_buffer(utki::span<const uint16_t> indices)
{
	return utki::make_shared<index_buffer>(indices);
}

utki::shared_ref<ruis::render::index_buffer> render_factory::create_index_buffer(utki::span<const uint32_t> indices)
{
	return utki::make_shared<index_buffer>(indices);
}

std::unique_ptr<ruis::render::render_factory::shaders> render_factory::create_shaders()
{
	auto ret = std::make_unique<ruis::render::render_factory::shaders>();
	// NOLINTNEXTLINE(bugprone-unused-return-value, "false positive")
	ret->pos_tex = std::make_unique<shader_pos_tex>();
	// NOLINTNEXTLINE(bugprone-unused-return-value, "false positive")
	ret->color_pos = std::make_unique<shader_color>();
	// NOLINTNEXTLINE(bugprone-unused-return-value, "false positive")
	ret->pos_clr = std::make_unique<shader_pos_clr>();
	// NOLINTNEXTLINE(bugprone-unused-return-value, "false positive")
	ret->color_pos_tex = std::make_unique<shader_color_pos_tex>();
	// NOLINTNEXTLINE(bugprone-unused-return-value, "false positive")
	ret->color_pos_tex_alpha = std::make_unique<shader_color_pos_tex_alpha>();
	// NOLINTNEXTLINE(bugprone-unused-return-value, "false positive")
	ret->color_pos_lum = std::make_unique<shader_color_pos_lum>();
	return ret;
}

utki::shared_ref<ruis::render::frame_buffer> render_factory::create_framebuffer( //
	std::shared_ptr<ruis::render::texture_2d> color,
	std::shared_ptr<ruis::render::texture_depth> depth,
	std::shared_ptr<ruis::render::texture_stencil> stencil
)
{
	return utki::make_shared<frame_buffer>(color, nullptr);
}
