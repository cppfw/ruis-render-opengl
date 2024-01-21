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

#include <memory>

#include <GL/glew.h>
#include <ruis/render/renderer.hpp>

#include "render_factory.hpp"

namespace ruis::render_opengl {

class renderer : public ruis::renderer
{
	GLuint default_framebuffer;

public:
	renderer(std::unique_ptr<render_factory> factory = std::make_unique<render_factory>());

	renderer(const renderer&) = delete;
	renderer& operator=(const renderer&) = delete;

	renderer(renderer&&) = delete;
	renderer& operator=(renderer&&) = delete;

	~renderer() override = default;

	void set_framebuffer_internal(ruis::frame_buffer* fb) override;

	void clear_framebuffer() override;

	bool is_scissor_enabled() const override;

	void set_scissor_enabled(bool enabled) override;

	r4::rectangle<int> get_scissor() const override;

	void set_scissor(r4::rectangle<int> r) override;

	r4::rectangle<int> get_viewport() const override;

	void set_viewport(r4::rectangle<int> r) override;

	void set_blend_enabled(bool enable) override;

	void set_blend_func(blend_factor src_color, blend_factor dst_color, blend_factor src_alpha, blend_factor dst_alpha)
		override;
};

} // namespace ruis::render_opengl
