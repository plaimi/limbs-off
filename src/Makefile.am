# Copyright (C) 2011 Stian Ellingsen <stian@plaimi.net>
# Copyright (C) 2012 Alexander Berntsen <alexander@plaimi.net>
#
# Copying and distribution of this file, with or without modification,
# are permitted in any medium without royalty provided the copyright
# notice and this notice are preserved.  This file is offered as-is,
# without any warranty.

## Process this file with automake to produce Makefile.in

limbs_off_datadir = $(datadir)/$(PACKAGE_NAME)

AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale/"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)/"\" \
	-DPACKAGE_DATA_DIR=\""${limbs_off_datadir}/"\" \
	-DPACKAGE_GFX_DIR=\""$(limbs_off_datadir)/data/graphics/"\" \
	-DPACKAGE_SFX_DIR=\""$(limbs_off_datadir)/data/sounds/"\" \
	-DPACKAGE_CFG_DIR=\""$(limbs_off_datadir)/config/"\" \
	$(FONTCONFIG_CFLAGS) \
	$(GL_CFLAGS) \
	$(PNG_CFLAGS) \
	$(SDL_CFLAGS)

AM_CFLAGS = \
	-Wall \
	-g

bin_PROGRAMS = limbs_off

limbs_off_SOURCES = \
	config_parser.h \
	config_parser.cpp \
	step_timer.h \
	step_timer.cpp \
	template_math.h \
	template_math_inl.h \
	geometry.h \
	geometry_inl.h \
	physics.h \
	physics_inl.h \
	physics/physics.cpp \
	physics/game_physics.h \
	physics/game_physics/game_physics.cpp \
	graphics.h \
	graphics/graphics.cpp \
	graphics/game_graphics_gl.h \
	graphics/game_graphics_gl/camera.cpp \
	graphics/game_graphics_gl/graphic.cpp \
	graphics/game_graphics_gl/screen.cpp \
	graphics/game_graphics_gl/texture_loader.cpp \
	menu.h \
	menu/menu.cpp \
	menu/submenu.cpp \
	menu/gui_element.cpp \
	character.h \
	character.cpp \
	actor.h \
	actor.cpp \
	player.h \
	player.cpp \
	game.h \
	game/game.cpp \
	bin/limbs_off.cpp

limbs_off_LDFLAGS = 

limbs_off_LDADD = \
	$(FONTCONFIG_LIBS) \
	$(GL_LIBS) \
	$(PNG_LIBS) \
	$(SDL_LIBS) -lSDL_ttf
