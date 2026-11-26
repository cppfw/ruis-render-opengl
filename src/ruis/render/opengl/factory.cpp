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

#include "factory.hpp"

#include <array>
#include <stdexcept>

#include <utki/debug.hpp>
#include <utki/log.hpp>
#include <utki/string.hpp>

#include "shaders/shader_color.hpp"
#include "shaders/shader_color_pos_lum.hpp"
#include "shaders/shader_color_pos_tex.hpp"
#include "shaders/shader_color_pos_tex_alpha.hpp"
#include "shaders/shader_pos_clr.hpp"
#include "shaders/shader_pos_tex.hpp"

#include "index_buffer.hpp"
#include "texture_2d.hpp"
#include "texture_cube.hpp"
#include "texture_depth.hpp"
#include "util.hpp"
#include "vertex_array.hpp"
#include "vertex_buffer.hpp"

using namespace std::string_view_literals;

using namespace ruis::render::opengl;

namespace {
void GLAPIENTRY opengl_error_callback(
	GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* user_param
)
{
	std::cout << "OpenGL" << (type == GL_DEBUG_TYPE_ERROR ? " ERROR" : "") << ": " << message << std::endl;
}
} // namespace

namespace {
utki::version_duplet parse_opengl_version(std::string_view version_string)
{
	utki::log_debug([&](auto& o) {
		o << "OpenGL version: " << version_string << std::endl;
	});

	utki::string_parser version_parser(version_string);

	auto major = version_parser.read_number<uint16_t>();

	// skip '.'
	utki::assert(version_parser.peek_char() == '.', SL);
	version_parser.read_char();

	auto minor = version_parser.read_number<uint16_t>();

	utki::log_debug([&](auto& o) {
		o << "Parsed OpenGL version: " << major << '.' << minor << std::endl;
	});

	return utki::version_duplet{.major = major, .minor = minor};
}
} // namespace

namespace {
utki::flags<ruis::render::opengl::extension> parse_supported_extensions(std::string_view extensions_string)
{
	utki::flags<ruis::render::opengl::extension> ext_flags = false;

	utki::log_debug([&](auto& o) {
		o << "OpenGL extensions: " << extensions_string << std::endl;
	});

	auto extensions = utki::split(extensions_string, ' ');
	for (auto& ext : extensions) {
		if (ext == "GL_EXT_texture_swizzle"sv) {
			ext_flags.set(ruis::render::opengl::extension::ext_texture_swizzle);
		} else if (ext == "GL_ARB_texture_swizzle"sv) {
			ext_flags.set(ruis::render::opengl::extension::arb_texture_swizzle);
		} else if (ext == "GL_ARB_debug_output"sv) {
			ext_flags.set(ruis::render::opengl::extension::arb_debug_output);
		} else if (ext == "GL_KHR_debug"sv) {
			ext_flags.set(ruis::render::opengl::extension::khr_debug);
		}
	}

	utki::log_debug([&](auto& o) {
		o << "Supported OpenGL extensions:" << std::endl;
		if (ext_flags.get(ruis::render::opengl::extension::ext_texture_swizzle)) {
			o << "  GL_EXT_texture_swizzle" << std::endl;
		}
		if (ext_flags.get(ruis::render::opengl::extension::arb_texture_swizzle)) {
			o << "  GL_ARB_texture_swizzle" << std::endl;
		}
		if (ext_flags.get(ruis::render::opengl::extension::arb_debug_output)) {
			o << "  GL_ARB_debug_output" << std::endl;
		}
		if (ext_flags.get(ruis::render::opengl::extension::khr_debug)) {
			o << "  GL_KHR_debug" << std::endl;
		}
	});

	return ext_flags;
}
} // namespace

factory::factory(utki::shared_ref<ruis::render::native_window> native_window) :
	native_window(std::move(native_window)),
	gl_version([&]() {
		// bind the headless GL context and leave it current so that object
		// creation (make_*) operates on the shared GL context.
		this->bind(this->native_window.get());

		auto version_string = std::string_view(
			// NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast, "needed to make string_view from GLubyte*")
			reinterpret_cast<const char*>(glGetString(GL_VERSION))
		);

		return parse_opengl_version(version_string);
	}()),
	supported_extensions([&]() {
		auto extensions_string = std::string_view(
			// NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast, "needed to make string_view from GLubyte*")
			reinterpret_cast<const char*>(glGetString(GL_EXTENSIONS))
		);

		return parse_supported_extensions(extensions_string);
	}())
{
	// the GL context is already bound by the gl_version member initializer.

	utki::run_debug([&]() {
		if (this->supported_extensions.get(ruis::render::opengl::extension::khr_debug)) {
			glEnable(GL_DEBUG_OUTPUT);
			glDebugMessageCallback(&opengl_error_callback, nullptr);
		}
	});

	glEnable(GL_CULL_FACE);
}

void factory::bind() const
{
	this->bind(this->native_window.get());
}

utki::shared_ref<ruis::render::factory::shaders> factory::make_shaders() const
{
	auto ret = utki::make_shared<ruis::render::factory::shaders>();
	// NOLINTNEXTLINE(bugprone-unused-return-value, "false positive")
	ret.get().pos_tex = std::make_unique<shader_pos_tex>(this->get_shared_ref());
	// NOLINTNEXTLINE(bugprone-unused-return-value, "false positive")
	ret.get().color_pos = std::make_unique<shader_color>(this->get_shared_ref());
	// NOLINTNEXTLINE(bugprone-unused-return-value, "false positive")
	ret.get().pos_clr = std::make_unique<shader_pos_clr>(this->get_shared_ref());
	// NOLINTNEXTLINE(bugprone-unused-return-value, "false positive")
	ret.get().color_pos_tex = std::make_unique<shader_color_pos_tex>(this->get_shared_ref());
	// NOLINTNEXTLINE(bugprone-unused-return-value, "false positive")
	ret.get().color_pos_tex_alpha = std::make_unique<shader_color_pos_tex_alpha>(this->get_shared_ref());
	// NOLINTNEXTLINE(bugprone-unused-return-value, "false positive")
	ret.get().color_pos_lum = std::make_unique<shader_color_pos_lum>(this->get_shared_ref());
	return ret;
}

utki::shared_ref<ruis::render::texture_2d> factory::make_texture_2d(
	rasterimage::format format, //
	rasterimage::dimensioned::dimensions_type dims,
	texture_2d_parameters params
) const
{
	return this->create_texture_2d_internal(
		format, //
		dims,
		{},
		std::move(params)
	);
}

utki::shared_ref<ruis::render::texture_2d> factory::make_texture_2d(
	const rasterimage::image_variant& imvar, //
	texture_2d_parameters params
) const
{
	auto imvar_copy = imvar;
	return this->make_texture_2d(
		std::move(imvar_copy), //
		std::move(params)
	);
}

utki::shared_ref<ruis::render::texture_2d> factory::make_texture_2d(
	rasterimage::image_variant&& imvar, //
	texture_2d_parameters params
) const
{
	auto iv = std::move(imvar);
	return std::visit(
		[this, &imvar = iv, &params](auto&& im) -> utki::shared_ref<ruis::render::texture_2d> {
			if constexpr (sizeof(im.pixels().front().front()) != 1) {
				throw std::logic_error(
					"factory::make_texture_2d(): "
					"non-8bit images are not supported"
				);
			} else {
				im.span().flip_vertical();
				auto data = im.pixels();
				return this->create_texture_2d_internal(
					imvar.get_format(),
					im.dims(),
					utki::make_span(data.front().data(), data.size_bytes()),
					std::move(params)
				);
			}
		},
		iv.variant
	);
}

utki::shared_ref<ruis::render::texture_2d> factory::create_texture_2d_internal(
	rasterimage::format type, //
	rasterimage::dimensioned::dimensions_type dims,
	utki::span<const uint8_t> data,
	texture_2d_parameters params
) const
{
	return utki::make_shared<texture_2d>(
		this->get_shared_ref(), //
		type,
		dims,
		data,
		params
	);
}

utki::shared_ref<ruis::render::texture_depth> factory::make_texture_depth( //
	rasterimage::dimensioned::dimensions_type dims
) const
{
	return utki::make_shared<texture_depth>(
		this->get_shared_ref(), //
		dims
	);
}

utki::shared_ref<ruis::render::texture_cube> factory::make_texture_cube(
	rasterimage::image_variant&& positive_x,
	rasterimage::image_variant&& negative_x,
	rasterimage::image_variant&& positive_y,
	rasterimage::image_variant&& negative_y,
	rasterimage::image_variant&& positive_z,
	rasterimage::image_variant&& negative_z
) const
{
	constexpr auto num_cube_sides = 6;
	std::array<rasterimage::image_variant, num_cube_sides> sides = {
		std::move(positive_x),
		std::move(negative_x),

		// negative_y and positive_y are swapped due to texture coordinates y-axis going downwards
		std::move(negative_y),
		std::move(positive_y),

		std::move(positive_z),
		std::move(negative_z)
	};
	std::array<ruis::render::opengl::texture_cube::cube_face_image, num_cube_sides> faces;

	auto face = faces.begin();
	for (auto& side : sides) {
		utki::assert(face != faces.end(), SL);
		std::visit(
			[&](auto& im) {
				if constexpr (sizeof(im.pixels().front().front()) != 1) {
					throw std::logic_error(
						"factory::make_texture_cube(): "
						"non-8bit images are not supported"
					);
				} else {
					im.span().flip_vertical();
					auto data = im.pixels();

					face->type = side.get_format();
					face->dims = im.dims();
					face->data = utki::make_span(data.front().data(), data.size_bytes());
				}
			},
			side.variant
		);
		++face;
	}

	return utki::make_shared<texture_cube>(
		this->get_shared_ref(), //
		faces
	);
}

utki::shared_ref<ruis::render::vertex_buffer> factory::make_vertex_buffer( //
	utki::span<const r4::vector4<float>> vertices
) const
{
	return utki::make_shared<vertex_buffer>(
		this->get_shared_ref(), //
		vertices
	);
}

utki::shared_ref<ruis::render::vertex_buffer> factory::make_vertex_buffer( //
	utki::span<const r4::vector3<float>> vertices
) const
{
	return utki::make_shared<vertex_buffer>(
		this->get_shared_ref(), //
		vertices
	);
}

utki::shared_ref<ruis::render::vertex_buffer> factory::make_vertex_buffer( //
	utki::span<const r4::vector2<float>> vertices
) const
{
	return utki::make_shared<vertex_buffer>(
		this->get_shared_ref(), //
		vertices
	);
}

utki::shared_ref<ruis::render::vertex_buffer> factory::make_vertex_buffer( //
	utki::span<const float> vertices
) const
{
	return utki::make_shared<vertex_buffer>(
		this->get_shared_ref(), //
		vertices
	);
}

utki::shared_ref<ruis::render::vertex_array> factory::make_vertex_array(
	std::vector<utki::shared_ref<const ruis::render::vertex_buffer>> buffers, //
	utki::shared_ref<const ruis::render::index_buffer> indices,
	ruis::render::vertex_array::mode mode
) const
{
	return utki::make_shared<vertex_array>(
		this->get_shared_ref(), //
		std::move(buffers),
		std::move(indices),
		mode
	);
}

utki::shared_ref<ruis::render::index_buffer> factory::make_index_buffer( //
	utki::span<const uint16_t> indices
) const
{
	return utki::make_shared<index_buffer>(
		this->get_shared_ref(), //
		indices
	);
}

utki::shared_ref<ruis::render::index_buffer> factory::make_index_buffer( //
	utki::span<const uint32_t> indices
) const
{
	return utki::make_shared<index_buffer>(
		this->get_shared_ref(), //
		indices
	);
}
