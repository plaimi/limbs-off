dnl Process this file with autoconf to produce a configure script.

AC_INIT(limbs_off, 0.1)
m4_ifdef([AM_SILENT_RULES],[AM_SILENT_RULES([yes])])

AM_INIT_AUTOMAKE([subdir-objects])
AC_CONFIG_HEADERS([config.h])

AC_PROG_CXX

AM_PROG_LIBTOOL

SDLCONFIG="sdl-config"
AC_ARG_WITH(sdl-config,
[[  --with-sdl-config=FILE
    Use the given path to sdl-config when determining
    SDL configuration; defaults to "sdl-config"]],
[
    if test "$withval" != "yes" -a "$withval" != ""; then
        SDLCONFIG=$withval
    fi
])

if test -z "$with_sdl"; then
	if $SDLCONFIG --version >/dev/null 2>&1; then
		with_sdl=yes
		SDL_CFLAGS=`$SDLCONFIG --cflags`
		SDL_LIBS=`$SDLCONFIG --libs`
	else
		with_sdl=no
	fi
fi
AM_CONDITIONAL(HAVE_LIBSDL, test "x$with_sdl" = "xyes")
AC_SUBST(SDL_CFLAGS)
AC_SUBST(SDL_LIBS)

PKG_CHECK_MODULES(FONTCONFIG, [fontconfig >= 2.8])
PKG_CHECK_MODULES(GL, [gl >= 7])
PKG_CHECK_MODULES(PNG, [libpng >= 1.2])

AC_CONFIG_FILES([Makefile])

AC_OUTPUT
