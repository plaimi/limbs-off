## Process this file with automake to produce Makefile.in

AM_CPPFLAGS = \
	      -I$(srcdir)/../src \
	      $(SDL_CFLAG)

AM_CFLAGS =\
	 -Wall\
	 -g

bin_PROGRAMS = bin/input_bindings

bin_input_bindings_SOURCES = \
			input_bindings.cpp \
			../src/input_handler.h \
			../src/input_handler.cpp \
			../src/init.h \
			../src/init.cpp

bin_input_bindings_LDFLAGS = 

bin_input_bindings_LDADD = $(SDL_LIBS)
