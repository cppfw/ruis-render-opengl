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

#include "frame_buffer.hpp"

#include <vector>

#include <GL/glew.h>
#include <utki/string.hpp>

#include "texture_2d.hpp"
#include "texture_depth.hpp"
#include "util.hpp"

using namespace ruis::render::opengl;

frame_buffer::frame_buffer(
	const utki::shared_ref<ruis::render::context>& rendering_context,
	std::shared_ptr<ruis::render::texture_2d> color,
	std::shared_ptr<ruis::render::texture_depth> depth,
	std::shared_ptr<ruis::render::texture_stencil> stencil
) :
	ruis::render::frame_buffer( //
		rendering_context,
		std::move(color),
		std::move(depth),
		std::move(stencil)
	)
{
	// In OpenGL framebuffer objects are not shared between contexts,
	// so make sure the owning context is bound when deleting the framebuffer object.
	this->rendering_context.get().apply([this]() {
		glGenFramebuffers(1, &this->fbo);
		assert_opengl_no_error();

		// No need to initialize the variable because it is initialized via
		// output argument of glGetIntegerv().
		// NOLINTNEXTLINE(cppcoreguidelines-init-variables)
		GLint old_fb;
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, &old_fb);

		glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);
		assert_opengl_no_error();

		if (this->color) {
			utki::assert(dynamic_cast<texture_2d*>(this->color.get()), SL);
			// NOLINTNEXTLINE(cppcoreguidelines-pro-type-static-cast-downcast)
			auto& tex = static_cast<texture_2d&>(*this->color);

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex.tex, 0);
			assert_opengl_no_error();
		} else {
			// TODO: glDrawBuffer(GL_NONE) ? See https://gamedev.stackexchange.com/a/152047
		}

		if (this->depth) {
			utki::assert(dynamic_cast<texture_depth*>(this->depth.get()), SL);
			// NOLINTNEXTLINE(cppcoreguidelines-pro-type-static-cast-downcast)
			auto& tex = static_cast<texture_depth&>(*this->depth);

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, tex.tex, 0);
			assert_opengl_no_error();
		}

		if (this->stencil) {
			throw std::logic_error("frame_buffer(): OpenGL stencil texture support is not implemented");
		}

		// check for completeness
		{
			GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
			assert_opengl_no_error();
			if (status != GL_FRAMEBUFFER_COMPLETE) {
				throw std::runtime_error(
					utki::cat("frame_buffer(): OpenGL framebuffer is incomplete: status = ", unsigned(status))
				);
			}
		}

		glBindFramebuffer(GL_FRAMEBUFFER, old_fb);
		assert_opengl_no_error();
	});
}

frame_buffer::~frame_buffer()
{
	// In OpenGL framebuffer objects are not shared between contexts,
	// so make sure the owning context is bound when deleting the framebuffer object.
	this->rendering_context.get().apply([this]() {
		glDeleteFramebuffers(1, &this->fbo);
		assert_opengl_no_error();
	});
}

rasterimage::image_variant frame_buffer::get_color_pixels()
{
	// Reading pixels back from a framebuffer object requires GL 3.0 core or the
	// ARB_framebuffer_object extension — the same capability the renderer already relies on to
	// create and render into framebuffer objects. There is deliberately no FBO-free fallback:
	// without that capability the color attachment cannot be read back at all.
	if (!GLEW_VERSION_3_0 && !GLEW_ARB_framebuffer_object) {
		throw std::runtime_error(
			"opengl::frame_buffer::get_color_pixels(): "
			"reading pixels from a framebuffer object requires GL 3.0 or the ARB_framebuffer_object extension"
		);
	}

	std::vector<uint8_t> buf(static_cast<size_t>(this->dims().x()) * this->dims().y() * sizeof(uint32_t));

	// glReadPixels on the framebuffer is the readback path. apply() binds this FBO and sets the
	// viewport to its size, restoring both afterwards. We deliberately do not call glReadBuffer():
	// an FBO with a single color attachment is read from color attachment zero directly, and a
	// GL_COLOR_ATTACHMENT* read-buffer target is not available in OpenGL 2.
	this->apply([&]() {
		glReadPixels(
			0, //
			0,
			GLsizei(this->dims().x()),
			GLsizei(this->dims().y()),
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			buf.data()
		);
	});
	assert_opengl_no_error();

	// glReadPixels returns the rows from the framebuffer's bottom edge upwards, so the captured
	// image is vertically flipped; restore the conventional top-down row order before handing it
	// out (both write_png and the probe transport expect top-down rows).
	auto im = rasterimage::image<uint8_t, sizeof(uint32_t)>::make(this->dims(), buf.data());
	im.span().flip_vertical();
	return {std::move(im)};
}
