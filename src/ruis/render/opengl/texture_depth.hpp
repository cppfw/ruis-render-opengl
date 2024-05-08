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

#pragma once

#include <ruis/render/texture_depth.hpp>

#include "opengl_texture.hpp"

namespace ruis::render::opengl {

class texture_depth :
	public opengl_texture, //
	public ruis::render::texture_depth
{
public:
	texture_depth(r4::vector2<uint32_t> dims);

	texture_depth(const texture_depth&) = delete;
	texture_depth& operator=(const texture_depth&) = delete;

	texture_depth(texture_depth&&) = delete;
	texture_depth& operator=(texture_depth&&) = delete;

	~texture_depth() override = default;
};

} // namespace ruis::render::opengl
