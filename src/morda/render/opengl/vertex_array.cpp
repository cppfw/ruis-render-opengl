#include "vertex_array.hpp"

#include "util.hpp"
#include "vertex_buffer.hpp"
#include "index_buffer.hpp"

using namespace morda::render_opengl;

vertex_array::vertex_array(std::vector<std::shared_ptr<morda::vertex_buffer>>&& buffers, std::shared_ptr<morda::index_buffer> indices, mode rendering_mode) :
		morda::vertex_array(std::move(buffers), std::move(indices), rendering_mode),
		vao([](){
			if(GLEW_ARB_vertex_array_object){
				GLuint ret;
				glGenVertexArrays(1, &ret);
				assert_opengl_no_error();
				return ret;
			}else{
				return GLuint(0);
			}
		}())
{
	if(GLEW_ARB_vertex_array_object){
		glBindVertexArray(this->vao);
		assert_opengl_no_error();

		this->bind_buffers();

		glBindVertexArray(0);
		assert_opengl_no_error();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		assert_opengl_no_error();
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		assert_opengl_no_error();
	}
}

vertex_array::~vertex_array(){
	if(GLEW_ARB_vertex_array_object){
		glBindVertexArray(0);
		assert_opengl_no_error();
		glDeleteVertexArrays(1, &this->vao);
		assert_opengl_no_error();
	}
}

void vertex_array::bind_buffers()const{
	for(unsigned i = 0; i != this->buffers.size(); ++i){
		ASSERT(dynamic_cast<vertex_buffer*>(this->buffers[i].operator->()))
		auto& vbo = static_cast<vertex_buffer&>(*this->buffers[i]);
		glBindBuffer(GL_ARRAY_BUFFER, vbo.buffer);
		assert_opengl_no_error();

//		TRACE(<< "vbo.numComponents = " << vbo.numComponents << " vbo.type = " << vbo.type << std::endl)

		glVertexAttribPointer(i, vbo.num_components, vbo.type, GL_FALSE, 0, nullptr);
		assert_opengl_no_error();

		glEnableVertexAttribArray(i);
		assert_opengl_no_error();
	}

	{
		ASSERT(dynamic_cast<index_buffer*>(this->indices.operator->()))
		auto& ivbo = static_cast<index_buffer&>(*this->indices);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ivbo.buffer);
		assert_opengl_no_error();
	}
}
