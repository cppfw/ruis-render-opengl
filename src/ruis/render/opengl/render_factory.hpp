/*
ruis-render-opengl - OpenGL GUI renderer

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

#pragma once

#include <ruis/render/render_factory.hpp>

namespace ruis::render_opengl {

class render_factory : public ruis::render_factory
{
public:
	render_factory();

	render_factory(const render_factory&) = delete;
	render_factory& operator=(const render_factory&) = delete;

	render_factory(render_factory&&) = delete;
	render_factory& operator=(render_factory&&) = delete;

	~render_factory() override = default;

	utki::shared_ref<texture_2d> create_texture_2d(
		rasterimage::format format,
		rasterimage::dimensioned::dimensions_type dims
	) override;

	utki::shared_ref<texture_2d> create_texture_2d(const rasterimage::image_variant& imvar) override;

	utki::shared_ref<ruis::vertex_buffer> create_vertex_buffer(utki::span<const r4::vector4<float>> vertices) override;
	utki::shared_ref<ruis::vertex_buffer> create_vertex_buffer(utki::span<const r4::vector3<float>> vertices) override;
	utki::shared_ref<ruis::vertex_buffer> create_vertex_buffer(utki::span<const r4::vector2<float>> vertices) override;
	utki::shared_ref<ruis::vertex_buffer> create_vertex_buffer(utki::span<const float> vertices) override;
	utki::shared_ref<ruis::index_buffer> create_index_buffer(utki::span<const uint16_t> indices) override;
	utki::shared_ref<ruis::vertex_array> create_vertex_array(
		std::vector<utki::shared_ref<const ruis::vertex_buffer>> buffers,
		const utki::shared_ref<const ruis::index_buffer>& indices,
		ruis::vertex_array::mode mode
	) override;

	std::unique_ptr<shaders> create_shaders() override;

	utki::shared_ref<ruis::frame_buffer> create_framebuffer(const utki::shared_ref<ruis::texture_2d>& color) override;

private:
	utki::shared_ref<ruis::texture_2d> create_texture_2d_internal(
		rasterimage::format type,
		rasterimage::dimensioned::dimensions_type dims,
		utki::span<const uint8_t> data
	);
};

} // namespace ruis::render_opengl
