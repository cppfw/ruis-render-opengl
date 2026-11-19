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
#include <rasterimage/image_variant.hpp>
#include <ruis/render/context.hpp>
#include <utki/flags.hpp>
#include <utki/shared.hpp>
#include <utki/version.hpp>

namespace ruis::render::opengl {

class context : public ruis::render::context
{
	GLuint default_framebuffer;

public:
	context(
		utki::shared_ref<ruis::render::native_window> native_window, //
		utki::shared_ref<const ruis::render::factory> factory
	);

	// ===============================
	// ====== factory functions ======

	utki::shared_ref<ruis::render::frame_buffer> make_framebuffer( //
		std::shared_ptr<ruis::render::texture_2d> color,
		std::shared_ptr<ruis::render::texture_depth> depth,
		std::shared_ptr<ruis::render::texture_stencil> stencil
	) override;

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
