/*
morda-render-opengl - OpenGL GUI renderer

Copyright (C) 2012-2023  Ivan Gagis <igagis@gmail.com>

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

#pragma once

#include <utki/span.hpp>

#include <morda/render/index_buffer.hpp>

#include "opengl_buffer.hpp"

namespace morda {
namespace render_opengl {

class index_buffer : public morda::index_buffer, public opengl_buffer {
public:
  const GLenum element_type;
  const GLsizei elements_count;

  index_buffer(utki::span<const uint16_t> indices);

  index_buffer(const index_buffer &) = delete;
  index_buffer &operator=(const index_buffer &) = delete;

private:
};

} // namespace render_opengl
} // namespace morda
