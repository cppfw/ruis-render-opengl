/*
ruis-render-opengl - OpenGL GUI renderer

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

#include "renderer.hpp"

#include <utki/config.hpp>

#include "frame_buffer.hpp"
#include "util.hpp"

using namespace ruis::render::opengl;

namespace {
unsigned get_max_texture_size()
{
	// the val variable is initialized via output argument, so no need to initialize
	// it here

	// NOLINTNEXTLINE(cppcoreguidelines-init-variables)
	GLint val;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &val);
	ASSERT(val > 0)
	return unsigned(val);
}
} // namespace

#ifdef DEBUG
namespace {
void GLAPIENTRY opengl_error_callback(
	GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* user_param
)
{
	std::cout << "OpenGL" << (type == GL_DEBUG_TYPE_ERROR ? " ERROR" : "") << ": " << message << std::endl;
}
} // namespace
#endif

renderer::renderer(std::unique_ptr<render_factory> factory) :
	ruis::render::renderer(std::move(factory), []() {
		renderer::params p;
		p.max_texture_size = get_max_texture_size();
		return p;
	}())
{
	LOG([](auto& o) {
		o << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
	})

	// On some platforms the default framebuffer is not 0, so because of this
	// check if default framebuffer value is saved or not everytime some
	// framebuffer is going to be bound and save the value if needed.

	// the old_fb variable is initialized via output argument, so no need to initialize
	// it here

	// NOLINTNEXTLINE(cppcoreguidelines-init-variables)
	GLint old_fb;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &old_fb);
	LOG([&](auto& o) {
		o << "old_fb = " << old_fb << std::endl;
	})
	this->default_framebuffer = GLuint(old_fb);

#ifdef DEBUG
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(opengl_error_callback, nullptr);
#endif

	glEnable(GL_CULL_FACE);
}

void renderer::set_framebuffer_internal(ruis::render::frame_buffer* fb)
{
	if (!fb) {
		glBindFramebuffer(GL_FRAMEBUFFER, this->default_framebuffer);
		assert_opengl_no_error();
		return;
	}

	ASSERT(dynamic_cast<frame_buffer*>(fb))
	// NOLINTNEXTLINE(cppcoreguidelines-pro-type-static-cast-downcast)
	auto& ogl_fb = static_cast<frame_buffer&>(*fb);

	glBindFramebuffer(GL_FRAMEBUFFER, ogl_fb.fbo);
	assert_opengl_no_error();
}

void renderer::clear_framebuffer()
{
	glClearColor(0, 0, 0, 1);
	assert_opengl_no_error();
	glClear(GL_COLOR_BUFFER_BIT);
	assert_opengl_no_error();

	// Default clear depth value is 1, see glClearDepth()
	glClear(GL_DEPTH_BUFFER_BIT);
	assert_opengl_no_error();

	// Default clear stencil value is 0, see glClearStencil()
	glClear(GL_STENCIL_BUFFER_BIT);
	assert_opengl_no_error();
}

r4::vector2<uint32_t> renderer::to_window_coords(ruis::vec2 point) const
{
	auto vp = this->get_viewport();

	point += ruis::vec2(1, 1);
	point = max(point, {0, 0}); // clamp to >= 0
	point /= 2;
	point.comp_multiply(vp.d.to<real>());
	point = round(point);
	return point.to<uint32_t>() + vp.p;
}

bool renderer::is_scissor_enabled() const
{
	return glIsEnabled(GL_SCISSOR_TEST) ? true : false; // "? true : false" is to avoid warning under MSVC
}

void renderer::set_scissor_enabled(bool enabled)
{
	if (enabled) {
		glEnable(GL_SCISSOR_TEST);
	} else {
		glDisable(GL_SCISSOR_TEST);
	}
}

r4::rectangle<uint32_t> renderer::get_scissor() const
{
	std::array<GLint, 4> osb{};
	glGetIntegerv(GL_SCISSOR_BOX, osb.data());

#ifdef DEBUG
	for (auto n : osb) {
		ASSERT(n >= 0)
	}
#endif

	return {
		uint32_t(osb[0]), //
		uint32_t(osb[1]),
		uint32_t(osb[2]),
		uint32_t(osb[3])
	};
}

void renderer::set_scissor(r4::rectangle<uint32_t> r)
{
	glScissor(r.p.x(), r.p.y(), r.d.x(), r.d.y());
	assert_opengl_no_error();
}

r4::rectangle<uint32_t> renderer::get_viewport() const
{
	std::array<GLint, 4> vp{};

	glGetIntegerv(GL_VIEWPORT, vp.data());

#ifdef DEBUG
	for (auto n : vp) {
		ASSERT(n >= 0)
	}
#endif

	return {
		uint32_t(vp[0]), //
		uint32_t(vp[1]),
		uint32_t(vp[2]),
		uint32_t(vp[3])
	};
}

void renderer::set_viewport(r4::rectangle<uint32_t> r)
{
	glViewport(r.p.x(), r.p.y(), r.d.x(), r.d.y());
	assert_opengl_no_error();
}

void renderer::set_blend_enabled(bool enable)
{
	if (enable) {
		glEnable(GL_BLEND);
	} else {
		glDisable(GL_BLEND);
	}
}

namespace {

const std::array<GLenum, size_t(ruis::render::renderer::blend_factor::enum_size)> blend_func = {
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

void renderer::set_blend_func(
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
