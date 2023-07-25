/*
morda-render-opengl - OpenGL GUI renderer

Copyright (C) 2012-2023  Ivan Gagis <igagis@gmail.com>

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

using namespace morda::render_opengl;

texture_2d::texture_2d(r4::vector2<float> dims) :
	morda::texture_2d(dims)
{
	glGenTextures(1, &this->tex);
	assert_opengl_no_error();
	ASSERT(this->tex != 0)
}

texture_2d::~texture_2d() noexcept
{
	glDeleteTextures(1, &this->tex);
}

void texture_2d::bind(unsigned unitNum) const
{
	glActiveTexture(GL_TEXTURE0 + unitNum);
	assert_opengl_no_error();
	glBindTexture(GL_TEXTURE_2D, this->tex);
	assert_opengl_no_error();
}
