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

#include <vector>

#include <GL/glew.h>
#include <rasterimage/image_variant.hpp>
#include <ruis/render/context.hpp>
#include <ruis/render/factory.hpp>
#include <utki/flags.hpp>
#include <utki/shared.hpp>
#include <utki/version.hpp>

#include "extension.hpp"

namespace ruis::render::opengl {

/**
 * @brief OpenGL render factory.
 *
 * The factory binds the shared (headless) GL context in its constructor and
 * leaves it current, so that all `make_*` calls operate on the shared GL
 * context.
 *
 * @par Why the factory retains a `native_window`
 * A GL context is always attached to a window (or, on some platforms, to a
 * hidden "headless" window); there is no such thing as a bare GL context that
 * is not tied to a window. The factory therefore retains the `native_window`
 * that holds the shared GL context, so it can bind it and keep the context
 * alive for all `make_*` calls. The abstract `ruis::render::factory` does not
 * retain a window; only the concrete renderer factories do, because they hold
 * the actual shared context.
 *
 * Whether a *dummy* window is required depends on the platform:
 * - Desktop (X11/Wayland): a dummy (hidden, 1x1) window is created solely to
 *   hold the shared GL context (see `shared_gl_context_native_window` in the
 *   glue); the real application windows share the context with it.
 * - Android: there is only one window (the app's window) and GLES has no
 *   headless context, so the factory uses the actual app window.
 */
class factory : public ruis::render::factory
{
public:
	// the native_window holds the actual shared (headless) GL context, so this
	// concrete factory retains it (the abstract ruis::render::factory does not)
	// to keep the context alive for all make_* calls. See the class comment for
	// why a (possibly dummy) window is required to hold the context.
	const utki::shared_ref<ruis::render::native_window> native_window;

	const utki::version_duplet gl_version;
	const utki::flags<extension> supported_extensions;

	factory(utki::shared_ref<ruis::render::native_window> native_window);

	// ===============================
	// ====== factory functions ======

	utki::shared_ref<shaders> make_shaders() const override;

	utki::shared_ref<ruis::render::texture_2d> make_texture_2d(
		rasterimage::format format,
		rasterimage::dimensioned::dimensions_type dims,
		texture_2d_parameters params
	) const override;

	utki::shared_ref<ruis::render::texture_2d> make_texture_2d(
		const rasterimage::image_variant& imvar,
		texture_2d_parameters params
	) const override;

	utki::shared_ref<ruis::render::texture_2d> make_texture_2d(
		rasterimage::image_variant&& imvar,
		texture_2d_parameters params
	) const override;

	utki::shared_ref<ruis::render::texture_depth> make_texture_depth( //
		rasterimage::dimensioned::dimensions_type dims
	) const override;

	utki::shared_ref<ruis::render::texture_cube> make_texture_cube(
		rasterimage::image_variant&& positive_x,
		rasterimage::image_variant&& negative_x,
		rasterimage::image_variant&& positive_y,
		rasterimage::image_variant&& negative_y,
		rasterimage::image_variant&& positive_z,
		rasterimage::image_variant&& negative_z
	) const override;

	utki::shared_ref<ruis::render::vertex_buffer> make_vertex_buffer( //
		utki::span<const r4::vector4<float>> vertices
	) const override;
	utki::shared_ref<ruis::render::vertex_buffer> make_vertex_buffer( //
		utki::span<const r4::vector3<float>> vertices
	) const override;
	utki::shared_ref<ruis::render::vertex_buffer> make_vertex_buffer( //
		utki::span<const r4::vector2<float>> vertices
	) const override;
	utki::shared_ref<ruis::render::vertex_buffer> make_vertex_buffer( //
		utki::span<const float> vertices
	) const override;
	utki::shared_ref<ruis::render::index_buffer> make_index_buffer( //
		utki::span<const uint16_t> indices
	) const override;
	utki::shared_ref<ruis::render::index_buffer> make_index_buffer( //
		utki::span<const uint32_t> indices
	) const override;
	utki::shared_ref<ruis::render::vertex_array> make_vertex_array(
		std::vector<utki::shared_ref<const ruis::render::vertex_buffer>> buffers, //
		utki::shared_ref<const ruis::render::index_buffer> indices,
		ruis::render::vertex_array::mode mode
	) const override;

private:
	utki::shared_ref<ruis::render::texture_2d> create_texture_2d_internal(
		rasterimage::format type, //
		rasterimage::dimensioned::dimensions_type dims,
		utki::span<const uint8_t> data,
		texture_2d_parameters params
	) const;
};

} // namespace ruis::render::opengl
