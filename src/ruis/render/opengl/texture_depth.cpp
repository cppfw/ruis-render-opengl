#include "texture_depth.hpp"

#include "util.hpp"

using namespace ruis::render::opengl;

texture_depth::texture_depth(r4::vector2<uint32_t> dims) :
	ruis::render::texture_depth(dims)
{
	this->bind(0);

	glTexImage2D( //
		GL_TEXTURE_2D,
		0, // 0th level, no mipmaps
		GL_DEPTH_COMPONENT, // TODO: GL_DEPTH_COMPONENT16? // internal format
		GLsizei(dims.x()),
		GLsizei(dims.y()),
		0, // border, should be 0!
		GL_DEPTH_COMPONENT, // format of the texel data
		GL_FLOAT, // TODO: GL_UNSIGNED_SHORT? // data type of the texel data
		nullptr // texel data
	);
	assert_opengl_no_error();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	assert_opengl_no_error();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	assert_opengl_no_error();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	assert_opengl_no_error();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	assert_opengl_no_error();
}