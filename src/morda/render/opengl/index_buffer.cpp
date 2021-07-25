#include "index_buffer.hpp"

#include "util.hpp"

#include <GL/glew.h>

using namespace morda::render_opengl;

index_buffer::index_buffer(utki::span<const uint16_t> indices) :
		element_type(GL_UNSIGNED_SHORT),
		elements_count(GLsizei(indices.size()))
{	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->buffer);
	assert_opengl_no_error();
	
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size_bytes(), &*indices.begin(), GL_STATIC_DRAW);
	assert_opengl_no_error();
}
