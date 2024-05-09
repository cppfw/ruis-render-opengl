#include "texture_cube.hpp"

#include "util.hpp"

using namespace ruis::render::opengl;

texture_cube::texture_cube(const std::array<cube_face_image, 6>& side_images)
{
	this->bind(0);

	unsigned i = 0;
	for (const auto& s : side_images) {
		auto format = this->set_swizzeling(s.type);
		glTexImage2D( //
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
			0, // 0th level, no mipmaps
			format, // internal format
			GLsizei(s.dims.x()),
			GLsizei(s.dims.y()),
			0, // border, should be 0
			format, // format of the texel data
			GL_UNSIGNED_BYTE,
			s.data.data()
		);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		++i;
	}
}

void texture_cube::bind(unsigned unit_num) const
{
	this->set_active_texture(unit_num);

	glBindTexture(GL_TEXTURE_CUBE_MAP, this->tex);
	assert_opengl_no_error();
}
