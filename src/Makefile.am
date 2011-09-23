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
	physics.h \
	physics.cpp \
	game_graphics_gl.h \
	game_graphics_gl/body_sprite.cpp \
	game_graphics_gl/screen.cpp \
	game_graphics_gl/texture_loader.cpp \
	bin/limbs_off.cpp

bin_limbs_off_LDFLAGS = 

bin_limbs_off_LDADD = $(GL_LIBS) $(SDL_LIBS) $(PNG_LIBS)
