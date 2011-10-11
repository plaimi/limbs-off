## Process this file with automake to produce Makefile.in

AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
	$(GL_CFLAGS) $(SDL_CFLAGS) $(PNG_CFLAGS)

AM_CFLAGS =\
	 -Wall\
	 -g

bin_PROGRAMS = bin/limbs_off

bin_limbs_off_SOURCES = \
	graphics.h \
	graphics.cpp \
	init.h \
	init.cpp \
	input_handler.h \
	input_handler.cpp \
	physics.h \
	physics.cpp \
	geometry.h \
	game_graphics_gl.h \
	game_graphics_gl/graphic.cpp \
	game_graphics_gl/screen.cpp \
	game_graphics_gl/texture_loader.cpp \
	game_physics.h \
	game_physics.cpp \
	bin/limbs_off.cpp

bin_limbs_off_LDFLAGS = 

bin_limbs_off_LDADD = $(GL_LIBS) $(SDL_LIBS) $(PNG_LIBS)
