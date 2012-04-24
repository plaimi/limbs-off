# Copyright (C) 2011 Stian Ellingsen <stian@plaimi.net>
# Copyright (C) 2012 Alexander Berntsen <alexander@plaimi.net>
#
# Copying and distribution of this file, with or without modification,
# are permitted in any medium without royalty provided the copyright
# notice and this notice are preserved.  This file is offered as-is,
# without any warranty.

## Process this file with automake to produce Makefile.in

nogit = test ! -d $(srcdir)/.git
git = git --work-tree=$(srcdir)

dist-hook all: ChangeLog

ChangeLog: .git-timestamp
	- $(nogit) || ($(git) log --date=short \
		--pretty="format:%ad  %an  <%ae>%n%w(76,8,8)%s%n%+b" > ChangeLog && \
		touch -r .git-timestamp ChangeLog)

.git-timestamp::
	- $(nogit) || touch -d "@`$(git) log -1 --format=%ct`" .git-timestamp

clean-local:
	$(nogit) || (test ChangeLog -nt .git-timestamp && \
		mv -f ChangeLog ChangeLog~ || rm -f ChangeLog);

limbs_off_datadir = $(datadir)/limbs-off


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
	$(SDL_CFLAGS) \
	-I$(srcdir)/src

AM_CFLAGS =\
	-Wall\
	-g

bin_PROGRAMS = limbs-off

limbs_off_SOURCES = \
	src/config_parser.h \
	src/config_parser.cpp \
	src/step_timer.h \
	src/step_timer.cpp \
	src/template_math.h \
	src/template_math_inl.h \
	src/geometry.h \
	src/geometry_inl.h \
	src/physics.h \
	src/physics_inl.h \
	src/physics/physics.cpp \
	src/physics/game_physics.h \
	src/physics/game_physics/game_physics.cpp \
	src/graphics.h \
	src/graphics/graphics.cpp \
	src/graphics/game_graphics_gl.h \
	src/graphics/game_graphics_gl/camera.cpp \
	src/graphics/game_graphics_gl/graphic.cpp \
	src/graphics/game_graphics_gl/screen.cpp \
	src/graphics/game_graphics_gl/texture_loader.cpp \
	src/menu.h \
	src/menu/menu.cpp \
	src/menu/submenu.cpp \
	src/menu/gui_element.cpp \
	src/character.h \
	src/character.cpp \
	src/actor.h \
	src/actor.cpp \
	src/player.h \
	src/player.cpp \
	src/game.h \
	src/game/game.cpp \
	src/bin/limbs_off.cpp

limbs_off_LDFLAGS = 

limbs_off_LDADD = \
	$(FONTCONFIG_LIBS) \
	$(GL_LIBS) \
	$(PNG_LIBS) \
	$(SDL_LIBS) -lSDL_ttf

TESTS = 

check_PROGRAMS = 

nobase_dist_limbs_off_data_DATA = \
	config/controllers1.conf \
	config/controllers2.conf \
	config/controllers3.conf

icondir = @ICONDIR@
icon_DATA = desktop/limbs-off.svg

desktopdir = @DESKTOPDIR@
desktop_DATA = desktop/limbs-off.desktop
