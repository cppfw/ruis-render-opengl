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

	texture_2d(const texture_2d&) = default;
	texture_2d& operator=(const texture_2d&) = default;

	texture_2d(texture_2d&&) = default;
	texture_2d& operator=(texture_2d&&) = default;

	~texture_2d() override = default;
};

} // namespace ruis::render::opengl
