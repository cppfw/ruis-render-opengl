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
#include <utki/flags.hpp>

#include "context.hpp"

namespace ruis::render::opengl {

struct opengl_texture {
	GLuint tex = 0;

	opengl_texture();

	opengl_texture(const opengl_texture&) = delete;
	opengl_texture& operator=(const opengl_texture&) = delete;

	opengl_texture(opengl_texture&&) = delete;
	opengl_texture& operator=(opengl_texture&&) = delete;

	~opengl_texture();

	void bind(unsigned unit_num) const;

protected:
	void set_active_texture(unsigned unit_num) const;

	GLint set_swizzeling(
		rasterimage::format f, //
		const utki::flags<extension>& supported_extensions
	) const;
};

} // namespace ruis::render::opengl
