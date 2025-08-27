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

#include <GL/glew.h>
#include <utki/string.hpp>

#include "texture_2d.hpp"
#include "texture_depth.hpp"
#include "util.hpp"

using namespace ruis::render::opengl;

frame_buffer::frame_buffer(
	utki::shared_ref<ruis::render::context> rendering_context,
	std::shared_ptr<ruis::render::texture_2d> color,
	std::shared_ptr<ruis::render::texture_depth> depth,
	std::shared_ptr<ruis::render::texture_stencil> stencil
) :
	ruis::render::frame_buffer( //
		std::move(rendering_context),
		std::move(color),
		std::move(depth),
		std::move(stencil)
	)
{
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
		ASSERT(dynamic_cast<texture_2d*>(this->color.get()))
		// NOLINTNEXTLINE(cppcoreguidelines-pro-type-static-cast-downcast)
		auto& tex = static_cast<texture_2d&>(*this->color);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex.tex, 0);
		assert_opengl_no_error();
	} else {
		// TODO: glDrawBuffer(GL_NONE) ? See https://gamedev.stackexchange.com/a/152047
	}

	if (this->depth) {
		ASSERT(dynamic_cast<texture_depth*>(this->depth.get()))
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
