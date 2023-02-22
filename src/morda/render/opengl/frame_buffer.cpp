/*
morda-render-opengl - OpenGL GUI renderer

Copyright (C) 2012-2021  Ivan Gagis <igagis@gmail.com>

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
#include "texture_2d.hpp"

#include "util.hpp"

#include <GL/glew.h>

using namespace morda::render_opengl;

frame_buffer::frame_buffer(const utki::shared_ref<morda::texture_2d>& color) :
		morda::frame_buffer(color)
{
	glGenFramebuffers(1, &this->fbo);
	assert_opengl_no_error();
	
	GLint old_fb;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &old_fb);
	
	glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);
	assert_opengl_no_error();
	
	ASSERT(dynamic_cast<texture_2d*>(&this->color.get()))
	auto& tex = static_cast<texture_2d&>(this->color.get());
	
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex.tex, 0);
	assert_opengl_no_error();
	
	// check for completeness
#ifdef DEBUG
	{
		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		assert_opengl_no_error();
		ASSERT(status == GL_FRAMEBUFFER_COMPLETE)
	}
#endif
	
	glBindFramebuffer(GL_FRAMEBUFFER, old_fb);
	assert_opengl_no_error();
}

frame_buffer::~frame_buffer()noexcept{
	glDeleteFramebuffers(1, &this->fbo);
	assert_opengl_no_error();
}
