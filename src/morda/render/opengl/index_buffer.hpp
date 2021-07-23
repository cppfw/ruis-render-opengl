#pragma once

#include <utki/span.hpp>

#include <morda/render/index_buffer.hpp>

#include "opengl_buffer.hpp"

namespace morda{ namespace render_opengl{

class index_buffer :
		public morda::index_buffer,
		public opengl_buffer
{
public:
	const GLenum element_type;
	const GLsizei elements_count;
	
	index_buffer(utki::span<const uint16_t> indices);
	
	index_buffer(const index_buffer&) = delete;
	index_buffer& operator=(const index_buffer&) = delete;

private:

};

}}
