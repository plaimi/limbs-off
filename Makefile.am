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
	src/repeat.hxx \
	src/config_parser.hxx \
	src/config_parser.cxx \
	src/get_font.hxx \
	src/step_timer.hxx \
	src/step_timer.cxx \
	src/template_math.hxx \
	src/template_math_inl.hxx \
	src/geometry.hxx \
	src/geometry_inl.hxx \
	src/physics.hxx \
	src/physics_inl.hxx \
	src/physics/physics.cxx \
	src/physics/game_physics.hxx \
	src/physics/game_physics/game_physics.cxx \
	src/graphics.hxx \
	src/graphics/graphics.cxx \
	src/graphics/game_graphics_gl.hxx \
	src/graphics/game_graphics_gl/camera.cxx \
	src/graphics/game_graphics_gl/graphic.cxx \
	src/graphics/game_graphics_gl/screen.cxx \
	src/graphics/game_graphics_gl/texture_loader.cxx \
	src/event_code.hxx \
	src/screen_element.hxx \
	src/screen_element.cxx \
	src/menu.hxx \
	src/menu/menu.cxx \
	src/menu/submenu.cxx \
	src/event_handler.hxx \
	src/collision_handler.hxx \
	src/collision_handler.cxx \
	src/character.hxx \
	src/character.cxx \
	src/action.hxx \
	src/actor.hxx \
	src/actor.cxx \
	src/player.hxx \
	src/player.cxx \
	src/game.hxx \
	src/game/game.cxx \
	src/game_loop.hxx \
	src/game_loop.cxx \
	src/bin/limbs_off.cxx

limbs_off_LDFLAGS = 

limbs_off_LDADD = \
	$(FONTCONFIG_LIBS) \
	$(GL_LIBS) \
	$(PNG_LIBS) \
	$(SDL_LIBS)

TESTS = 

check_PROGRAMS = 

nobase_dist_limbs_off_data_DATA = \
	config/controllers1.conf \
	config/controllers2.conf \
	config/controllers3.conf

limbs_off_icondir = @ICONDIR@
dist_limbs_off_icon_DATA = desktop/limbs-off.svg

limbs_off_desktopdir = @DESKTOPDIR@
dist_limbs_off_desktop_DATA = desktop/limbs-off.desktop
