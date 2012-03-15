dnl Process this file with autoconf to produce a configure script.

AC_INIT(limbs_off, 0.1)
m4_ifdef([AM_SILENT_RULES],[AM_SILENT_RULES([yes])])

AM_INIT_AUTOMAKE([subdir-objects])
AC_CONFIG_HEADERS([config.h])

AC_PROG_CXX

AM_PROG_LIBTOOL

PKG_CHECK_MODULES(FONTCONFIG, [fontconfig >= 2.8])
PKG_CHECK_MODULES(GL, [gl >= 7])
PKG_CHECK_MODULES(PNG, [libpng >= 1.2])
PKG_CHECK_MODULES(SDL, [sdl >= 1.2.10 SDL_ttf >= 2.0.10])

AC_CONFIG_FILES([Makefile])

AC_OUTPUT
