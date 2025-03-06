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

#include "shader_base.hpp"

#include <vector>

#include <GL/glew.h>
#include <utki/debug.hpp>
#include <utki/string.hpp>

#include "index_buffer.hpp"
#include "util.hpp"
#include "vertex_array.hpp"
#include "vertex_buffer.hpp"

using namespace ruis::render::opengl;

const std::array<GLenum, size_t(ruis::render::vertex_array::mode::enum_size)> shader_base::mode_map = {
	GL_TRIANGLES, // TRIANGLES
	GL_TRIANGLE_FAN, // TRIANGLE_FAN
	GL_LINE_LOOP, // LINE_LOOP
	GL_TRIANGLE_STRIP // TRIANGLE_STRIP
};

namespace {
// return true if not compiled
bool check_for_compile_errors(GLuint shader)
{
	GLint value = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &value);
	if (value == 0) { // if not compiled
		GLint log_len = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_len);
		if (log_len > 1) { // 1 char is a terminating 0
			std::vector<char> log(log_len);
			// 'len' variable is used to get value via output argument, so no need to
			// initialize it

			// NOLINTNEXTLINE(cppcoreguidelines-init-variables)
			GLint len;
			glGetShaderInfoLog(shader, GLsizei(log.size()), &len, log.data());
			utki::log([&](auto& o) {
				o << "===Compile log===\n" << log.data() << std::endl;
			});
		} else {
			utki::log([&](auto& o) {
				o << "Shader compile log is empty" << std::endl;
			});
		}
		return true;
	}
	return false;
}

// return true if not linked
bool check_for_link_errors(GLuint program)
{
	GLint value = 0;
	glGetProgramiv(program, GL_LINK_STATUS, &value);
	if (value == 0) { // if not linked
		GLint log_len = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_len);
		if (log_len > 1) { // 1 is for terminating 0 character
			std::vector<char> log(log_len);
			// 'len' variable is used to get value via output argument, so no need to
			// initialize it

			// NOLINTNEXTLINE(cppcoreguidelines-init-variables)
			GLint len;
			glGetProgramInfoLog(program, GLsizei(log.size()), &len, log.data());
			utki::log([&](auto& o) {
				o << "===Link log===\n" << log.data() << std::endl;
			});
		}
		return true;
	}
	return false;
}

} // namespace

shader_wrapper::shader_wrapper(const char* code, GLenum type) :
	s([&type]() {
		auto s = glCreateShader(type);
		if (s == 0) {
			throw std::runtime_error("glCreateShader() failed");
		}
		return s;
	}())
{
	const char* c = code;

	glShaderSource(this->s, 1, &c, nullptr);
	glCompileShader(this->s);
	if (check_for_compile_errors(this->s)) {
		utki::log([&](auto& o) {
			o << "Error while compiling:\n" << c << std::endl;
		});
		glDeleteShader(this->s);
		throw std::logic_error("compiling shader failed");
	}
}

program_wrapper::program_wrapper(const char* vertex_shader_code, const char* fragment_shader_code) :
	vertex_shader(vertex_shader_code, GL_VERTEX_SHADER),
	fragment_shader(fragment_shader_code, GL_FRAGMENT_SHADER),
	p(glCreateProgram())
{
	glAttachShader(this->p, vertex_shader.s);
	glAttachShader(this->p, fragment_shader.s);

	// the variable is initialized via output argument, so no need to initialize
	// it here

	// NOLINTNEXTLINE(cppcoreguidelines-init-variables)
	GLint max_num_attribs;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &max_num_attribs);
	ASSERT(max_num_attribs >= 0)

	for (GLuint i = 0; i < GLuint(max_num_attribs); ++i) {
		std::stringstream ss;
		ss << "a" << i;
		//		TRACE(<< ss.str() << std::endl)
		glBindAttribLocation(this->p, i, ss.str().c_str());
		assert_opengl_no_error();
	}

	glLinkProgram(this->p);
	if (check_for_link_errors(this->p)) {
		LOG([&](auto& o) {
			o << "Error while linking shader program" << vertex_shader_code << std::endl
			  << fragment_shader_code << std::endl;
		})
		glDeleteProgram(this->p);
		throw std::logic_error("linking shader program failed");
	}
}

shader_base::shader_base(const char* vertex_shader_code, const char* fragment_shader_code) :
	program(vertex_shader_code, fragment_shader_code),
	matrix_uniform(this->get_uniform("matrix"))
{}

GLint shader_base::get_uniform(const char* n)
{
	GLint ret = glGetUniformLocation(this->program.p, n);
	if (ret < 0) {
		throw std::logic_error(utki::cat("no uniform found in the shader program: ", n));
	}
	return ret;
}

void shader_base::render(const r4::matrix4<float>& m, const ruis::render::vertex_array& va) const
{
	ASSERT(this->is_bound())

	ASSERT(dynamic_cast<const index_buffer*>(&va.indices.get()))
	// NOLINTNEXTLINE(cppcoreguidelines-pro-type-static-cast-downcast)
	const auto& ivbo = static_cast<const index_buffer&>(va.indices.get());

	this->set_matrix(m);

	ASSERT(dynamic_cast<const vertex_array*>(&va))
	// NOLINTNEXTLINE(cppcoreguidelines-pro-type-static-cast-downcast)
	const auto& ogl_va = static_cast<const vertex_array&>(va);

	ogl_va.bind_buffers();

	//	TRACE(<< "ivbo.elementsCount = " << ivbo.elementsCount << "
	// ivbo.elementType = " << ivbo.elementType << std::endl)

	glDrawElements(mode_to_gl_mode(va.rendering_mode), ivbo.elements_count, ivbo.element_type, nullptr);
	assert_opengl_no_error();
}
