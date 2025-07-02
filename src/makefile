include prorab.mk
include prorab-license.mk
include prorab-clang-format.mk
include prorab-install-dbgsrc.mk

$(eval $(call prorab-config, ../config))

this_src_dir := ruis

this_name := ruis-render-opengl$(this_dbg)

this_soname := $(shell cat $(d)soname.txt)

this_srcs += $(call prorab-src-dir, $(this_src_dir))

ifeq ($(os), linux)
    this_ldlibs += -l GL
    this_ldlibs += -l GLEW
else ifeq ($(os), windows)
    this_ldlibs += -l opengl32
    this_ldlibs += -l glew32
else ifeq ($(os), macosx)
    this_ldlibs += -l GLEW
    this_ldlibs += -framework OpenGL
endif

this_ldlibs += -l ruis$(this_dbg)
this_ldlibs += -l utki$(this_dbg)

$(eval $(prorab-build-lib))

this_license_file := ../LICENSE
$(eval $(prorab-license))

$(eval $(prorab-clang-format))

$(eval $(prorab-install-dbgsrc))
