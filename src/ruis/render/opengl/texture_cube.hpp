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
		rasterimage::format type;
		rasterimage::dimensioned::dimensions_type dims;
		utki::span<const uint8_t> data;
	};

	texture_cube(const std::array<cube_face_image, 6>& side_images);

	texture_cube(const texture_cube&) = delete;
	texture_cube& operator=(const texture_cube&) = delete;

	texture_cube(texture_cube&&) = delete;
	texture_cube& operator=(texture_cube&&) = delete;

	~texture_cube() override = default;

	void bind(unsigned unit_num) const;
};

} // namespace ruis::render::opengl
