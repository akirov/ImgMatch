
#ROOT_SRC_DIR=$(CURDIR)
#ROOT_SRC_DIR=$(dir $(realpath $(lastword $(MAKEFILE_LIST))))

include ./defines.mk

export USE_OPENCV
export OPENCVLIB
export OPENCVINC
export OPENCVVER

#export IMAGEIMP
CXXFLAGS+=-D$(IMAGEIMP)
export CXXFLAGS

export COREDIR=core
export UIDIR=qtui
export BINDIR=bin
export BUILDDIR=build


ALLDIRS=$(COREDIR) $(UIDIR)


.PHONY: $(ALLDIRS) all clean


all: $(ALLDIRS)


$(COREDIR):
	$(MAKE) -C $@


$(UIDIR):
ifneq (,$(findstring qt, $(UIDIR)))
	cd $(UIDIR); qmake CONFIG+=$(IMAGEIMP); $(MAKE)
else
	$(error Don't know how to make UIDIR = $(UIDIR))
endif


clean:
	-@for dir in $(ALLDIRS); do $(MAKE) -C $$dir $@; done
#	-@for dir in $(ALLDIRS); do ( cd $$dir; $(MAKE) $@ ); done
