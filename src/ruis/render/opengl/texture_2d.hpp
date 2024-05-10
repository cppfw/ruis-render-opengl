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

#include <ruis/render/render_factory.hpp>
#include <ruis/render/texture_2d.hpp>

#include "opengl_texture.hpp"

namespace ruis::render::opengl {

class texture_2d :
	public opengl_texture, //
	public ruis::render::texture_2d
{
public:
	texture_2d(
		rasterimage::format type,
		rasterimage::dimensioned::dimensions_type dims,
		utki::span<const uint8_t> data,
		ruis::render::render_factory::texture_2d_parameters params
	);

	texture_2d(const texture_2d&) = delete;
	texture_2d& operator=(const texture_2d&) = delete;

	texture_2d(texture_2d&&) = delete;
	texture_2d& operator=(texture_2d&&) = delete;

	~texture_2d() override = default;
};

} // namespace ruis::render::opengl
