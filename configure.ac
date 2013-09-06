# Copyright (C) 2011 Stian Ellingsen <stian@plaimi.net>
# Copyright (C) 2012 Alexander Berntsen <alexander@plaimi.net>
#
# Copying and distribution of this file, with or without modification,
# are permitted in any medium without royalty provided the copyright
# notice and this notice are preserved.  This file is offered as-is,
# without any warranty.

dnl Process this file with autoconf to produce a configure script.

AC_INIT([limbs-off], [0.1], [https://secure.plaimi.net/bugs])
m4_ifdef([AM_SILENT_RULES],[AM_SILENT_RULES([yes])])

AM_INIT_AUTOMAKE([subdir-objects])
AC_CONFIG_HEADERS([config.h])

# Check for programs
AC_PROG_CXX
AM_PROG_LIBTOOL
AC_ARG_WITH(
    [sdl-config],
    [AS_HELP_STRING([--with-sdl-config=FILE], [Use the given path to 
        sdl-config when determining SDL configuration])],
    [SDLCONFIG="$withval"]
    [AC_SUBST([SDLCONFIG])],
    [AC_PATH_PROG([SDLCONFIG], sdl-config)]
)

# Check whether we actually have SDLCONFIG
AS_IF(
    [test "x$SDLCONFIG" = "x"],
    [AC_MSG_ERROR([sdl-config not found and not defined!])]
)

# Set SDL flags
SDL_CFLAGS="`$SDLCONFIG --cflags`"
SDL_LIBS="$($SDLCONFIG --libs)"

# Check for additional SDL libraries
AC_SEARCH_LIBS([TTF_Init], [SDL_ttf],, AC_MSG_ERROR([missing sdl-ttf!]))

SDL_LIBS="${SDL_LIBS} -lSDL_ttf"
AC_SUBST([SDL_CFLAGS])
AC_SUBST([SDL_LIBS])

# Check other dependencies
PKG_CHECK_MODULES(FONTCONFIG, [fontconfig >= 2.8])
PKG_CHECK_MODULES(GL, [gl >= 7])
PKG_CHECK_MODULES(PNG, [libpng >= 1.2])

# Makefile
AC_CONFIG_FILES([Makefile])

# Configure-switch for verbose game output
AC_ARG_ENABLE(
    [verbose],
    [AC_HELP_STRING([--enable-verbose], [verbose game output])],
    [enable_verbose=$enableval],
    [enable_verbose="yes"]
)

# Set VERBOSE var now
AS_IF(
    [test "x$enable_verbose" = "xyes"],
    [AC_DEFINE(VERBOSE, 1, verbose mode)],
    [AC_DEFINE(VERBOSE, 0, verbose mode)]
)

# Let user specify icondir
AC_ARG_WITH(
    [icondir],
    [AS_HELP_STRING([--with-icondir=DIR], [path to icon directory (default: 
        $datadir/pixmaps)])],
    [ICONDIR="$withval"],
    [ICONDIR='$(datadir)/pixmaps']
)

# Let user specify desktopdir
AC_ARG_WITH(
    [desktopdir],
    [AS_HELP_STRING([--with-desktopdir=DIR], [path to desktop directory 
        (default: $datadir/applications)])],
    [DESKTOPDIR="$withval"],
    [DESKTOPDIR='$(datadir)/applications']
)

AC_SUBST([DESKTOPDIR])
AC_SUBST([ICONDIR])

AC_OUTPUT

echo
echo
echo Limbs Off.................................... : Version $VERSION
echo Prefix....................................... : $prefix
echo Icondir...................................... : $ICONDIR
echo Desktopdir................................... : $DESKTOPDIR
if test "x$enable_verbose" = "xyes"; then
    echo Verbose...................................... : Yes
else
    echo Verbose...................................... : No
fi
