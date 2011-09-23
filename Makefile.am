## Process this file with automake to produce Makefile.in

dist-hook: ChangeLog

ChangeLog: .git-timestamp
	- git log --date=short --pretty="format:%ad  %an  <%ae>%n%w(76,8,8)%s%n%b" \
	> ChangeLog
	touch -r .git-timestamp ChangeLog

.git-timestamp::
	-touch -d "@`git log -1 --format=%ct`" .git-timestamp

clean-local:
	test ChangeLog -nt .git-timestamp && mv -f ChangeLog ChangeLog~ || \
		rm -f ChangeLog;

SUBDIRS = src
