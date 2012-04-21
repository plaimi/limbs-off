# Copyright (C) 2011 Stian Ellingsen <stian@plaimi.net>
# Copyright (C) 2012 Alexander Berntsen <alexander@plaimi.net>
#
# Copying and distribution of this file, with or without modification,
# are permitted in any medium without royalty provided the copyright
# notice and this notice are preserved.  This file is offered as-is,
# without any warranty.

## Process this file with automake to produce Makefile.in

SUBDIRS = \
	  src \
	  config \
	  desktop

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

TESTS = 

check_PROGRAMS = 

doc_DATA = \
	   README \
	   AUTHORS \
	   ChangeLog \
	   HACKING
