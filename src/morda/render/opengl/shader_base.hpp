#pragma once

#include <utki/config.hpp>
#include <utki/debug.hpp>

#include <r4/matrix.hpp>

#include <vector>

#include "util.hpp"

#include <morda/render/vertex_array.hpp>

#include <GL/glew.h>

namespace morda{ namespace render_opengl{

struct shader_wrapper{
	GLuint s;
	shader_wrapper(const char* code, GLenum type);
	~shader_wrapper()noexcept{
		glDeleteShader(this->s);
	}
};

struct program_wrapper{
	shader_wrapper vertexShader;
	shader_wrapper fragmentShader;
	GLuint p;
	program_wrapper(const char* vertexShaderCode, const char* fragmentShaderCode);

	virtual ~program_wrapper()noexcept{
		glDeleteProgram(this->p);
	}
};

class shader_base{
	program_wrapper program;
	
	const GLint matrixUniform;
	
	static const shader_base* boundShader;
public:
	shader_base(const char* vertexShaderCode, const char* fragmentShaderCode);
	
	shader_base(const shader_base&) = delete;
	shader_base& operator=(const shader_base&) = delete;
	
	virtual ~shader_base()noexcept{}

protected:
	GLint get_uniform(const char* n);
	
	void bind()const{
		glUseProgram(program.p);
		assertOpenGLNoError();
		boundShader = this;
	}
	
	bool is_bound()const noexcept{
		return this == boundShader;
	}
	
	void set_uniform_matrix4f(GLint id, const r4::matrix4<float>& m)const{
		glUniformMatrix4fv(id, 1, GL_TRUE, reinterpret_cast<const GLfloat*>(&m));
		assertOpenGLNoError();
	}
	
	void set_uniform4f(GLint id, float x, float y, float z, float a)const{
		glUniform4f(id, x, y, z, a);
		assertOpenGLNoError();
	}
	
	void set_matrix(const r4::matrix4<float>& m)const{
		this->set_uniform_matrix4f(this->matrixUniform, m);
	}
	
	static GLenum mode_map[];
	
	static GLenum mode_to_gl_mode(morda::vertex_array::mode mode){
		return mode_map[unsigned(mode)];
	}
	
	void render(const r4::matrix4<float>& m, const morda::vertex_array& va)const;
};

}}
