## Process this file with automake to produce Makefile.in

nogit = test ! -d .git

dist-hook all: ChangeLog

ChangeLog: .git-timestamp
	- $(nogit) || (git log --date=short \
		--pretty="format:%ad  %an  <%ae>%n%w(76,8,8)%s%n%+b" > ChangeLog && \
		touch -r .git-timestamp ChangeLog)

.git-timestamp::
	- $(nogit) || touch -d "@`git log -1 --format=%ct`" .git-timestamp

clean-local:
	test ChangeLog -nt .git-timestamp && mv -f ChangeLog ChangeLog~ || \
		rm -f ChangeLog;

AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
	$(GL_CFLAGS) $(SDL_CFLAGS) $(PNG_CFLAGS) \
	-I$(srcdir)/src

AM_CFLAGS =\
	 -Wall\
	 -g

bin_PROGRAMS = limbs_off

limbs_off_SOURCES = \
	src/graphics.h \
	src/graphics.cpp \
	src/init.h \
	src/init.cpp \
	src/input_handler.h \
	src/input_handler.cpp \
	src/physics.h \
	src/physics_inl.h \
	src/physics.cpp \
	src/geometry.h \
	src/geometry_inl.h \
	src/template_math.h \
	src/template_math_inl.h \
	src/game_graphics_gl.h \
	src/game_graphics_gl/graphic.cpp \
	src/game_graphics_gl/screen.cpp \
	src/game_graphics_gl/texture_loader.cpp \
	src/game_physics.h \
	src/game_physics.cpp \
	src/bin/limbs_off.cpp

limbs_off_LDFLAGS = 

limbs_off_LDADD = $(GL_LIBS) $(SDL_LIBS) $(PNG_LIBS)

TESTS = test/input_bindings

check_PROGRAMS = test/input_bindings

test_input_bindings_SOURCES = \
			test/input_bindings.cpp \
			src/action.h \
			src/input_handler.h \
			src/input_handler.cpp \
			src/init.h \
			src/init.cpp

test_input_bindings_LDFLAGS = 

test_input_bindings_LDADD = $(SDL_LIBS)

limbs_off_datadir = $(datadir)/limbs-off

dist_limbs_off_data_DATA = src/controllers.conf
