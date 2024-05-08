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

	~texture_depth() = default;
};

} // namespace ruis::render::opengl
