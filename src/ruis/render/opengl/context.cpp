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

#include "context.hpp"

#include <array>

#include <utki/shared.hpp>

#include "frame_buffer.hpp"
#include "util.hpp"

using namespace ruis::render::opengl;

context::context(
	utki::shared_ref<ruis::render::native_window> native_window, //
	utki::shared_ref<const ruis::render::factory> factory
) :
	ruis::render::context(
		std::move(native_window), //
		std::move(factory), //
		// clang-format off
		{
			.initial_matrix = ruis::mat4()
				// OpenGL identity matrix:
				//   viewport edges: left = -1, right = 1, top = 1, bottom = -1
				//   z-axis towards viewer
				.set_identity()
				// x-axis right, y-axis down, z-axis away
				.scale(1, -1, -1)
				// viewport edges: left = 0, top = 0
				.translate(-1, -1)
				// viewport edges: right = 1, bottom = 1
				.scale(2, 2)
		} // clang-format on
	)
{
	this->apply([&]() {
		// On some platforms the default framebuffer is not 0, so because of this
		// check if default framebuffer value is saved or not everytime some
		// framebuffer is going to be bound and save the value if needed.

		// the old_fb variable is initialized via output argument, so no need to initialize
		// it here

		// NOLINTNEXTLINE(cppcoreguidelines-init-variables)
		GLint old_fb;
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, &old_fb);
		this->default_framebuffer = GLuint(old_fb);

		glEnable(GL_CULL_FACE);
	});
}

utki::shared_ref<ruis::render::frame_buffer> context::make_framebuffer(
	std::shared_ptr<ruis::render::texture_2d> color, //
	std::shared_ptr<ruis::render::texture_depth> depth,
	std::shared_ptr<ruis::render::texture_stencil> stencil
)
{
	return utki::make_shared<frame_buffer>( //
		this->get_shared_ref(),
		std::move(color),
		std::move(depth),
		std::move(stencil)
	);
}

void context::set_framebuffer_internal(ruis::render::frame_buffer* fb)
{
	if (!fb) {
		glBindFramebuffer(GL_FRAMEBUFFER, this->default_framebuffer);
		assert_opengl_no_error();
		return;
	}

	utki::assert(dynamic_cast<frame_buffer*>(fb), SL);
	// NOLINTNEXTLINE(cppcoreguidelines-pro-type-static-cast-downcast)
	auto& ogl_fb = static_cast<frame_buffer&>(*fb);

	glBindFramebuffer(GL_FRAMEBUFFER, ogl_fb.fbo);
	assert_opengl_no_error();
}

void context::clear_framebuffer_color()
{
	// Default clear color is RGBA = (0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	assert_opengl_no_error();
}

void context::clear_framebuffer_depth()
{
	// Default clear depth value is 1, see glClearDepth()
	glClear(GL_DEPTH_BUFFER_BIT);
	assert_opengl_no_error();
}

void context::clear_framebuffer_stencil()
{
	// Default clear stencil value is 0, see glClearStencil()
	glClear(GL_STENCIL_BUFFER_BIT);
	assert_opengl_no_error();
}

r4::vector2<uint32_t> context::to_window_coords(const ruis::vec2& point) const
{
	auto vp = this->get_viewport();

	auto p = point + ruis::vec2(1, 1);
	p = max(p, {0, 0}); // clamp to >= 0
	p /= 2;
	p.comp_multiply(vp.d.to<real>());
	p = round(p);
	return p.to<uint32_t>() + vp.p;
}

bool context::is_scissor_enabled() const noexcept
{
	return glIsEnabled(GL_SCISSOR_TEST) ? true : false; // "? true : false" is to avoid warning under MSVC
}

void context::enable_scissor(bool enable)
{
	if (enable) {
		glEnable(GL_SCISSOR_TEST);
	} else {
		glDisable(GL_SCISSOR_TEST);
	}
}

r4::rectangle<uint32_t> context::get_scissor() const
{
	std::array<GLint, 4> osb{};
	glGetIntegerv(GL_SCISSOR_BOX, osb.data());

#ifdef DEBUG
	for (auto n : osb) {
		utki::assert(n >= 0, SL);
	}
#endif

	return {
		uint32_t(osb[0]), //
		uint32_t(osb[1]),
		uint32_t(osb[2]),
		uint32_t(osb[3])
	};
}

void context::set_scissor(const r4::rectangle<uint32_t>& r)
{
	glScissor(
		GLint(r.p.x()), //
		GLint(r.p.y()),
		GLint(r.d.x()),
		GLint(r.d.y())
	);
	assert_opengl_no_error();
}

r4::rectangle<uint32_t> context::get_viewport() const
{
	std::array<GLint, 4> vp{};

	glGetIntegerv(GL_VIEWPORT, vp.data());

#ifdef DEBUG
	for (auto n : vp) {
		utki::assert(n >= 0, SL);
	}
#endif

	return {
		uint32_t(vp[0]), //
		uint32_t(vp[1]),
		uint32_t(vp[2]),
		uint32_t(vp[3])
	};
}

void context::set_viewport(const r4::rectangle<uint32_t>& r)
{
	glViewport(
		GLint(r.p.x()), //
		GLint(r.p.y()),
		GLint(r.d.x()),
		GLint(r.d.y())
	);
	assert_opengl_no_error();
}

void context::enable_blend(bool enable)
{
	if (enable) {
		glEnable(GL_BLEND);
	} else {
		glDisable(GL_BLEND);
	}
}

namespace {

const std::array<GLenum, size_t(ruis::render::context::blend_factor::enum_size)> blend_func = {
	GL_ZERO,
	GL_ONE,
	GL_SRC_COLOR,
	GL_ONE_MINUS_SRC_COLOR,
	GL_DST_COLOR,
	GL_ONE_MINUS_DST_COLOR,
	GL_SRC_ALPHA,
	GL_ONE_MINUS_SRC_ALPHA,
	GL_DST_ALPHA,
	GL_ONE_MINUS_DST_ALPHA,
	GL_CONSTANT_COLOR,
	GL_ONE_MINUS_CONSTANT_COLOR,
	GL_CONSTANT_ALPHA,
	GL_ONE_MINUS_CONSTANT_ALPHA,
	GL_SRC_ALPHA_SATURATE
};

} // namespace

void context::set_blend_func(
	blend_factor src_color,
	blend_factor dst_color,
	blend_factor src_alpha,
	blend_factor dst_alpha
)
{
	glBlendFuncSeparate(
		// NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-constant-array-index)
		blend_func[unsigned(src_color)],
		// NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-constant-array-index)
		blend_func[unsigned(dst_color)],
		// NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-constant-array-index)
		blend_func[unsigned(src_alpha)],
		// NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-constant-array-index)
		blend_func[unsigned(dst_alpha)]
	);
}

bool context::is_depth_enabled() const noexcept
{
	return glIsEnabled(GL_DEPTH_TEST) ? true : false; // "? true : false" is to avoid warning under MSVC
}

void context::enable_depth(bool enable)
{
	if (enable) {
		glEnable(GL_DEPTH_TEST);
	} else {
		glDisable(GL_DEPTH_TEST);
	}
}
