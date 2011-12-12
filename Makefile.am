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
	src/actor.h \
	src/actor.cpp \
	src/character.h \
	src/character.cpp \
	src/graphics.h \
	src/graphics.cpp \
	src/init.h \
	src/init.cpp \
	src/event_handler.h \
	src/physics.h \
	src/physics_inl.h \
	src/physics.cpp \
	src/player.h \
	src/player.cpp \
	src/geometry.h \
	src/geometry_inl.h \
	src/template_math.h \
	src/template_math_inl.h \
	src/step_timer.h \
	src/step_timer.cpp \
	src/game_graphics_gl.h \
	src/game_graphics_gl/camera.cpp \
	src/game_graphics_gl/graphic.cpp \
	src/game_graphics_gl/screen.cpp \
	src/game_graphics_gl/texture_loader.cpp \
	src/game_physics.h \
	src/game_physics.cpp \
	src/bin/limbs_off.cpp

limbs_off_LDFLAGS = 

limbs_off_LDADD = $(GL_LIBS) $(SDL_LIBS) $(PNG_LIBS)

TESTS = 

check_PROGRAMS = 

limbs_off_datadir = $(datadir)/limbs-off

dist_limbs_off_data_DATA = src/controllers.conf
