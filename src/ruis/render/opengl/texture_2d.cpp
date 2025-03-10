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

#include "texture_2d.hpp"

#include "util.hpp"

using namespace ruis::render::opengl;

texture_2d::texture_2d(
	utki::shared_ref<ruis::render::context> render_context, //
	rasterimage::format type,
	rasterimage::dimensioned::dimensions_type dims,
	utki::span<const uint8_t> data,
	ruis::render::context::texture_2d_parameters params
) :
	ruis::render::texture_2d(
		std::move(render_context), //
		dims
	)
{
	ASSERT(data.size() % rasterimage::to_num_channels(type) == 0)
	ASSERT(data.size() % dims.x() == 0)
	ASSERT(data.size() == 0 || data.size() / rasterimage::to_num_channels(type) / dims.x() == dims.y())

	this->bind(0);

	GLint internal_format = this->set_swizzeling(type);

	// we will be passing pixels to OpenGL which are 1-byte aligned.
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	assert_opengl_no_error();

	glTexImage2D(
		GL_TEXTURE_2D,
		0, // 0th level, no mipmaps
		internal_format, // internal format
		GLsizei(dims.x()),
		GLsizei(dims.y()),
		0, // border, should be 0!
		internal_format, // format of the texel data
		GL_UNSIGNED_BYTE, // data type of the texel data
		data.size() == 0 ? nullptr : data.data() // texel data
	);
	assert_opengl_no_error();

	if (!data.empty() && params.mipmap != texture_2d::mipmap::none) {
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	auto to_gl_filter = [](texture_2d::filter f) {
		switch (f) {
			case texture_2d::filter::nearest:
				return GL_NEAREST;
			case texture_2d::filter::linear:
				return GL_LINEAR;
		}
		return GL_NEAREST;
	};

	GLint mag_filter = to_gl_filter(params.mag_filter);

	GLint min_filter = [&]() {
		switch (params.mipmap) {
			case texture_2d::mipmap::none:
				return to_gl_filter(params.min_filter);
			case texture_2d::mipmap::nearest:
				switch (params.min_filter) {
					case texture_2d::filter::nearest:
						return GL_NEAREST_MIPMAP_NEAREST;
					case texture_2d::filter::linear:
						return GL_LINEAR_MIPMAP_NEAREST;
				}
				break;
			case texture_2d::mipmap::linear:
				switch (params.min_filter) {
					case texture_2d::filter::nearest:
						return GL_NEAREST_MIPMAP_LINEAR;
					case texture_2d::filter::linear:
						return GL_LINEAR_MIPMAP_LINEAR;
				}
				break;
		}
		return GL_NEAREST;
	}();

	// It is necessary to set filter parameters for every texture. Otherwise it may not work.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
	assert_opengl_no_error();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);
	assert_opengl_no_error();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	assert_opengl_no_error();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	assert_opengl_no_error();
}
