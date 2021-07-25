#include <utki/debug.hpp>

#include <vector>

#include "shader_base.hpp"
#include "util.hpp"
#include "vertex_array.hpp"
#include "index_buffer.hpp"
#include "vertex_buffer.hpp"

#include <GL/glew.h>

using namespace morda::render_opengl;

const shader_base* shader_base::boundShader = nullptr;

GLenum shader_base::mode_map[] = {
	GL_TRIANGLES,			// TRIANGLES
	GL_TRIANGLE_FAN,		// TRIANGLE_FAN
	GL_LINE_LOOP,			// LINE_LOOP
	GL_TRIANGLE_STRIP		// TRIANGLE_STRIP
};

namespace{
// return true if not compiled
bool checkForCompileErrors(GLuint shader) {
	GLint value = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &value);
	if (value == 0) { //if not compiled
		GLint logLen = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);
		if (logLen > 1) {//1 char is a terminating 0
			std::vector<char> log(logLen);
			GLint len;
			glGetShaderInfoLog(shader, GLsizei(log.size()), &len, &*log.begin());
			TRACE( << "===Compile log===\n" << &*log.begin() << std::endl)
		} else {
			TRACE( << "Shader compile log is empty" << std::endl)
		}
		return true;
	}
	return false;
}

// return true if not linked
bool checkForLinkErrors(GLuint program){
	GLint value = 0;
	glGetProgramiv(program, GL_LINK_STATUS, &value);
	if(value == 0){ //if not linked
		GLint logLen = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLen);
		if(logLen > 1){ //1 is for terminating 0 character.
			std::vector<char> log(logLen);
			GLint len;
			glGetProgramInfoLog(program, GLsizei(log.size()), &len, &*log.begin());
			TRACE(<< "===Link log===\n" << &*log.begin() << std::endl)
		}
		return true;
	}
	return false;
}

}

shader_wrapper::shader_wrapper(const char* code, GLenum type) {
	this->s = glCreateShader(type);

	if (this->s == 0) {
		throw std::runtime_error("glCreateShader() failed");
	}

	const char* c = code;

	glShaderSource(this->s, 1, &c, 0);
	glCompileShader(this->s);
	if (checkForCompileErrors(this->s)) {
		TRACE( << "Error while compiling:\n" << c << std::endl)
		glDeleteShader(this->s);
		throw std::logic_error("compiling shader failed");
	}
}

program_wrapper::program_wrapper(const char* vertexShaderCode, const char* fragmentShaderCode) :
		vertexShader(vertexShaderCode, GL_VERTEX_SHADER),
		fragmentShader(fragmentShaderCode, GL_FRAGMENT_SHADER)
{
	this->p = glCreateProgram();
	glAttachShader(this->p, vertexShader.s);
	glAttachShader(this->p, fragmentShader.s);
	
	GLint maxAttribs;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxAttribs);
	ASSERT(maxAttribs >= 0)
	
	for(GLuint i = 0; i < GLuint(maxAttribs); ++i){
		std::stringstream ss;
		ss << "a" << i;
//		TRACE(<< ss.str() << std::endl)
		glBindAttribLocation(this->p, i, ss.str().c_str());
		assert_opengl_no_error();
	}
	
	glLinkProgram(this->p);
	if (checkForLinkErrors(this->p)) {
		TRACE( << "Error while linking shader program" << vertexShaderCode << std::endl << fragmentShaderCode << std::endl)
		glDeleteProgram(this->p);
		throw std::logic_error("linking shader program failed");
	}
}

shader_base::shader_base(const char* vertexShaderCode, const char* fragmentShaderCode) :
		program(vertexShaderCode, fragmentShaderCode),
		matrixUniform(this->get_uniform("matrix"))
{}

GLint shader_base::get_uniform(const char* n) {
	GLint ret = glGetUniformLocation(this->program.p, n);
	if(ret < 0){
		throw std::logic_error("no uniform found in the shader program");
	}
	return ret;
}

void shader_base::render(const r4::matrix4<float>& m, const morda::vertex_array& va)const{
	ASSERT(this->is_bound())
	
	ASSERT(dynamic_cast<const index_buffer*>(va.indices.operator ->()))
	const index_buffer& ivbo = static_cast<const index_buffer&>(*va.indices);
	
	this->set_matrix(m);
	
	for(unsigned i = 0; i != va.buffers.size(); ++i){
		ASSERT(dynamic_cast<vertex_buffer*>(va.buffers[i].operator->()))
		auto& vbo = static_cast<vertex_buffer&>(*va.buffers[i]);
		glBindBuffer(GL_ARRAY_BUFFER, vbo.buffer);
		assert_opengl_no_error();
		
//		TRACE(<< "vbo.numComponents = " << vbo.numComponents << " vbo.type = " << vbo.type << std::endl)
		
		glVertexAttribPointer(i, vbo.num_components, vbo.type, GL_FALSE, 0, nullptr);
		assert_opengl_no_error();
		
		glEnableVertexAttribArray(i);
		assert_opengl_no_error();
	}
	
	{
		ASSERT(dynamic_cast<index_buffer*>(va.indices.operator->()))
		auto& ivbo = static_cast<index_buffer&>(*va.indices);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ivbo.buffer);
		assert_opengl_no_error();
	}
	
//	TRACE(<< "ivbo.elementsCount = " << ivbo.elementsCount << " ivbo.elementType = " << ivbo.elementType << std::endl)
	
	glDrawElements(mode_to_gl_mode(va.rendering_mode), ivbo.elements_count, ivbo.element_type, nullptr);
	assert_opengl_no_error();
}
