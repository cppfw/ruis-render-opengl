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
#include <ruis/render/context.hpp>
#include <utki/flags.hpp>
#include <utki/shared.hpp>
#include <utki/version.hpp>

namespace ruis::render::opengl {

enum class extension {
	ext_texture_swizzle,
	arb_texture_swizzle = ext_texture_swizzle,
	arb_debug_output,
	khr_debug,

	enum_size
};

class context : public ruis::render::context
{
	GLuint default_framebuffer;

public:
	const utki::version_duplet gl_version;

	const utki::flags<extension> supported_extensions;

	context(utki::shared_ref<ruis::render::native_window> native_window);

	// ===============================
	// ====== factory functions ======

	utki::shared_ref<shaders> make_shaders() const override;

	utki::shared_ref<ruis::render::texture_2d> make_texture_2d(
		rasterimage::format format,
		rasterimage::dimensioned::dimensions_type dims,
		texture_2d_parameters params
	) const override;

	utki::shared_ref<ruis::render::texture_2d> make_texture_2d(
		const rasterimage::image_variant& imvar,
		texture_2d_parameters params
	) const override;

	utki::shared_ref<ruis::render::texture_2d> make_texture_2d(
		rasterimage::image_variant&& imvar,
		texture_2d_parameters params
	) const override;

	utki::shared_ref<ruis::render::texture_depth> make_texture_depth( //
		rasterimage::dimensioned::dimensions_type dims
	) const override;

	utki::shared_ref<ruis::render::texture_cube> make_texture_cube(
		rasterimage::image_variant&& positive_x,
		rasterimage::image_variant&& negative_x,
		rasterimage::image_variant&& positive_y,
		rasterimage::image_variant&& negative_y,
		rasterimage::image_variant&& positive_z,
		rasterimage::image_variant&& negative_z
	) const override;

	utki::shared_ref<ruis::render::vertex_buffer> make_vertex_buffer( //
		utki::span<const r4::vector4<float>> vertices
	) const override;
	utki::shared_ref<ruis::render::vertex_buffer> make_vertex_buffer( //
		utki::span<const r4::vector3<float>> vertices
	) const override;
	utki::shared_ref<ruis::render::vertex_buffer> make_vertex_buffer( //
		utki::span<const r4::vector2<float>> vertices
	) const override;
	utki::shared_ref<ruis::render::vertex_buffer> make_vertex_buffer( //
		utki::span<const float> vertices
	) const override;
	utki::shared_ref<ruis::render::index_buffer> make_index_buffer( //
		utki::span<const uint16_t> indices
	) const override;
	utki::shared_ref<ruis::render::index_buffer> make_index_buffer( //
		utki::span<const uint32_t> indices
	) const override;
	utki::shared_ref<ruis::render::vertex_array> make_vertex_array(
		std::vector<utki::shared_ref<const ruis::render::vertex_buffer>> buffers, //
		utki::shared_ref<const ruis::render::index_buffer> indices,
		ruis::render::vertex_array::mode mode
	) const override;

	utki::shared_ref<ruis::render::frame_buffer> make_framebuffer( //
		std::shared_ptr<ruis::render::texture_2d> color,
		std::shared_ptr<ruis::render::texture_depth> depth,
		std::shared_ptr<ruis::render::texture_stencil> stencil
	) override;

private:
	utki::shared_ref<ruis::render::texture_2d> create_texture_2d_internal(
		rasterimage::format type,
		rasterimage::dimensioned::dimensions_type dims,
		utki::span<const uint8_t> data,
		texture_2d_parameters params
	) const;

public:
	// =====================================
	// ====== state control functions ======

	void set_framebuffer_internal(ruis::render::frame_buffer* fb) override;

	void clear_framebuffer_color() override;

	void clear_framebuffer_depth() override;

	void clear_framebuffer_stencil() override;

	r4::vector2<uint32_t> to_window_coords(const ruis::vec2& point) const override;

	bool is_scissor_enabled() const noexcept override;

	void enable_scissor(bool enable) override;

	r4::rectangle<uint32_t> get_scissor() const override;

	void set_scissor(const r4::rectangle<uint32_t>& r) override;

	r4::rectangle<uint32_t> get_viewport() const override;

	void set_viewport(const r4::rectangle<uint32_t>& r) override;

	void enable_blend(bool enable) override;

	void set_blend_func(
		blend_factor src_color, //
		blend_factor dst_color,
		blend_factor src_alpha,
		blend_factor dst_alpha
	) override;

	bool is_depth_enabled() const noexcept override;

	void enable_depth(bool enable) override;
};

} // namespace ruis::render::opengl
