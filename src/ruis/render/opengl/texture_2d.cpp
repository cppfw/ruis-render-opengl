#include "texture_2d.hpp"

#include "util.hpp"

using namespace ruis::render::opengl;

texture_2d::texture_2d(
	rasterimage::format type,
	rasterimage::dimensioned::dimensions_type dims,
	utki::span<const uint8_t> data,
	ruis::render::render_factory::texture_2d_parameters params
) :
	ruis::render::texture_2d(dims)
{
	ASSERT(data.size() % rasterimage::to_num_channels(type) == 0)
	ASSERT(data.size() % dims.x() == 0)
	ASSERT(data.size() == 0 || data.size() / rasterimage::to_num_channels(type) / dims.x() == dims.y())

	this->bind(0);

	GLint internal_format = [&type]() {
		switch (type) {
			default:
				ASSERT(false)
			case rasterimage::format::grey:
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_R, GL_RED);
				assert_opengl_no_error();
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, GL_RED);
				assert_opengl_no_error();
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, GL_RED);
				assert_opengl_no_error();
				// GL_LUMINANCE is deprecated in OpenGL 3, so we use GL_RED
				return GL_RED;
			case rasterimage::format::greya:
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_R, GL_RED);
				assert_opengl_no_error();
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, GL_RED);
				assert_opengl_no_error();
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, GL_RED);
				assert_opengl_no_error();
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_A, GL_GREEN);
				assert_opengl_no_error();
				// GL_LUMINANCE_ALPHA is deprecated in OpenGL 3, so we use GL_RG
				return GL_RG;
			case rasterimage::format::rgb:
				return GL_RGB;
			case rasterimage::format::rgba:
				return GL_RGBA;
		}
	}();

	// we will be passing pixels to OpenGL which are 1-byte aligned.
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	assert_opengl_no_error();

	glTexImage2D(
		GL_TEXTURE_2D,
		0, // 0th level, no mipmaps
		internal_format, // internal format
		GLsizei(dims.x()),
		GLsizei(dims.y()),
		0, // border, should be 0!
		internal_format, // format of the texel data
		GL_UNSIGNED_BYTE, // data type of the texel data
		data.size() == 0 ? nullptr : data.data() // texel data
	);
	assert_opengl_no_error();

	if (!data.empty() && params.mipmap != texture_2d::mipmap::none) {
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	auto to_gl_filter = [](texture_2d::filter f) {
		switch (f) {
			case texture_2d::filter::nearest:
				return GL_NEAREST;
			case texture_2d::filter::linear:
				return GL_LINEAR;
		}
		return GL_NEAREST;
	};

	GLint mag_filter = to_gl_filter(params.mag_filter);

	GLint min_filter = [&]() {
		switch (params.mipmap) {
			case texture_2d::mipmap::none:
				return to_gl_filter(params.min_filter);
			case texture_2d::mipmap::nearest:
				switch (params.min_filter) {
					case texture_2d::filter::nearest:
						return GL_NEAREST_MIPMAP_NEAREST;
					case texture_2d::filter::linear:
						return GL_LINEAR_MIPMAP_NEAREST;
				}
				break;
			case texture_2d::mipmap::linear:
				switch (params.min_filter) {
					case texture_2d::filter::nearest:
						return GL_NEAREST_MIPMAP_LINEAR;
					case texture_2d::filter::linear:
						return GL_LINEAR_MIPMAP_LINEAR;
				}
				break;
		}
		return GL_NEAREST;
	}();

	// It is necessary to set filter parameters for every texture. Otherwise it may not work.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
	assert_opengl_no_error();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);
	assert_opengl_no_error();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	assert_opengl_no_error();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	assert_opengl_no_error();
}
