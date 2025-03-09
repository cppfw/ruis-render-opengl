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

#include <rasterimage/image_variant.hpp>
#include <ruis/render/texture_cube.hpp>

#include "opengl_texture.hpp"

namespace ruis::render::opengl {

class texture_cube :
	public opengl_texture, //
	public ruis::render::texture_cube
{
public:
	struct cube_face_image {
		rasterimage::format type{};
		rasterimage::dimensioned::dimensions_type dims;
		utki::span<const uint8_t> data;
	};

	constexpr static const auto num_cube_faces = 6;

	texture_cube(
		utki::shared_ref<ruis::render::context> render_context, //
		const std::array<cube_face_image, num_cube_faces>& side_images
	);

	texture_cube(const texture_cube&) = delete;
	texture_cube& operator=(const texture_cube&) = delete;

	texture_cube(texture_cube&&) = delete;
	texture_cube& operator=(texture_cube&&) = delete;

	~texture_cube() override = default;

	void bind(unsigned unit_num) const;
};

} // namespace ruis::render::opengl
