SUBDIRS = src doc

DIST_SUBDIRS = $(SUBDIRS)

ACLOCAL_AMFLAGS = -I m4

all-local:

anew: clean all


clean-local:
	$(RM) *stamp*
	$(RM) *~

dist-local:
	./update-version.sh
