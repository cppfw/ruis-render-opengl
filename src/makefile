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
    this_ldlibs += -lGL
    this_ldlibs += -lGLEW
else ifeq ($(os), windows)
    this_ldlibs += -lopengl32
    this_ldlibs += -lglew32
else ifeq ($(os), macosx)
    this_ldlibs += -lGLEW
    this_ldlibs += -framework OpenGL
endif

this_ldlibs += -lruis$(this_dbg)
this_ldlibs += -lutki$(this_dbg)

$(eval $(prorab-build-lib))

this_license_file := ../LICENSE
$(eval $(prorab-license))

$(eval $(prorab-clang-format))

$(eval $(prorab-install-dbgsrc))
