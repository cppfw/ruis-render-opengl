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
#include <utki/debug.hpp>

namespace ruis::render::opengl {

inline void assert_opengl_no_error()
{
#ifdef DEBUG
	GLenum error = glGetError();
	switch (error) {
		case GL_NO_ERROR:
			return;
		case GL_INVALID_ENUM:
			utki::assert(
				false,
				[](auto& o) {
					o << "OpenGL error: GL_INVALID_ENUM";
				},
				SL
			);
			break;
		case GL_INVALID_VALUE:
			utki::assert(
				false,
				[](auto& o) {
					o << "OpenGL error: GL_INVALID_VALUE";
				},
				SL
			);
			break;
		case GL_INVALID_OPERATION:
			utki::assert(
				false,
				[](auto& o) {
					o << "OpenGL error: GL_INVALID_OPERATION";
				},
				SL
			);
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			utki::assert(
				false,
				[](auto& o) {
					o << "OpenGL error: GL_INVALID_FRAMEBUFFER_OPERATION";
				},
				SL
			);
			break;
		case GL_OUT_OF_MEMORY:
			utki::assert(
				false,
				[](auto& o) {
					o << "OpenGL error: GL_OUT_OF_MEMORY";
				},
				SL
			);
			break;
		default:
			utki::assert(
				false,
				[&](auto& o) {
					o << "Unknown OpenGL error, code = " << int(error);
				},
				SL
			);
			break;
	}
#endif
}

} // namespace ruis::render::opengl
