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

#include "texture_cube.hpp"

#include "util.hpp"

using namespace ruis::render::opengl;

texture_cube::texture_cube(const std::array<cube_face_image, num_cube_faces>& side_images)
{
	this->bind(0);

	unsigned i = 0;
	for (const auto& s : side_images) {
		auto format = this->set_swizzeling(s.type);
		glTexImage2D( //
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
			0, // 0th level, no mipmaps
			format, // internal format
			GLsizei(s.dims.x()),
			GLsizei(s.dims.y()),
			0, // border, should be 0
			format, // format of the texel data
			GL_UNSIGNED_BYTE,
			s.data.data()
		);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		++i;
	}
}

void texture_cube::bind(unsigned unit_num) const
{
	this->set_active_texture(unit_num);

	glBindTexture(GL_TEXTURE_CUBE_MAP, this->tex);
	assert_opengl_no_error();
}
