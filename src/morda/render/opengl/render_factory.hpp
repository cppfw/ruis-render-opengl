/*
morda-render-opengl - OpenGL GUI renderer

Copyright (C) 2012-2021  Ivan Gagis <igagis@gmail.com>

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

#include <morda/render/render_factory.hpp>

namespace morda{ namespace render_opengl{

class render_factory : public morda::render_factory{
public:
	render_factory();
	
	render_factory(const render_factory&) = delete;
	render_factory& operator=(const render_factory&) = delete;
	
	virtual ~render_factory()noexcept;

	std::shared_ptr<morda::texture_2d> create_texture_2d(morda::texture_2d::type type, r4::vector2<unsigned> dims, utki::span<const uint8_t> data)override;

	std::shared_ptr<morda::vertex_buffer> create_vertex_buffer(utki::span<const r4::vector4<float>> vertices)override;
	
	std::shared_ptr<morda::vertex_buffer> create_vertex_buffer(utki::span<const r4::vector3<float>> vertices)override;
	
	std::shared_ptr<morda::vertex_buffer> create_vertex_buffer(utki::span<const r4::vector2<float>> vertices)override;

	std::shared_ptr<morda::vertex_buffer> create_vertex_buffer(utki::span<const float> vertices)override;
	
	std::shared_ptr<morda::index_buffer> create_index_buffer(utki::span<const uint16_t> indices)override;
	
	std::shared_ptr<morda::vertex_array> create_vertex_array(std::vector<std::shared_ptr<morda::vertex_buffer>>&& buffers, std::shared_ptr<morda::index_buffer> indices, morda::vertex_array::mode mode)override;

	std::unique_ptr<shaders> create_shaders()override;
	
	std::shared_ptr<morda::frame_buffer> create_framebuffer(std::shared_ptr<morda::texture_2d> color)override;
};

}}