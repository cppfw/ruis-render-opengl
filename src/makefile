include prorab.mk
include prorab-license.mk
include prorab-clang-format.mk
include prorab-install-dbgsrc.mk

$(eval $(call prorab-config, ../config))

this_name := ruis-render-opengl

this_soname := $(shell cat $(d)soname.txt)

this_srcs += $(call prorab-src-dir,.)

ifeq ($(os), linux)
    this_ldlibs += -lGL -lGLEW
else ifeq ($(os), windows)
    this_ldlibs += -lopengl32 -lglew32
else ifeq ($(os), macosx)
    this_ldlibs += -lGLEW -framework OpenGL
endif

this_ldlibs += -lruis

$(eval $(prorab-build-lib))

this_license_file := ../LICENSE
this_src_dir := ruis
$(eval $(prorab-license))

$(eval $(prorab-clang-format))

$(eval $(prorab-install-dbgsrc))
