
#ROOT_SRC_DIR=$(CURDIR)
#ROOT_SRC_DIR=$(dir $(realpath $(lastword $(MAKEFILE_LIST))))


include ./defines.mk

ifneq (,$(findstring OPENCV, $(SIFT_IMP)))
    IMAGEIMP=OCVIMAGE_8UC3
endif

ifneq (,$(findstring OCVIMAGE, $(IMAGEIMP)))
    USE_OPENCV=1
endif

ifneq (,$(findstring 1, $(USE_OPENCV)))
    ifneq (,$(OPENCVINC))
        CPPFLAGS+=-I$(OPENCVINC)
    endif

    ifneq (,$(OPENCVLIB))
        LDFLAGS+=-L$(OPENCVLIB)
    endif

    LDLIBS+=-lopencv_core$(OPENCVVER) -lopencv_highgui$(OPENCVVER) -lopencv_ml$(OPENCVVER) -lopencv_imgproc$(OPENCVVER)
endif

CPPFLAGS+=-D$(IMAGEIMP)

ifneq (,$(findstring 1, $(USE_CPP11)))
	CXXFLAGS+=-std=c++11
	QT_CPP11_FLAGS=QMAKE_CXXFLAGS+=-std=c++11
endif

ifneq (,$(findstring 1, $(ENABLE_LOG)))
	CPPFLAGS+=-DENABLE_LOG
endif


export IMAGEIMP
export USE_OPENCV
export OPENCVLIB
export OPENCVINC
export OPENCVVER
export SIFT_IMP

export CPPFLAGS
export CXXFLAGS
export LDFLAGS
export LDLIBS

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
	cd $(UIDIR); qmake CONFIG+=$(IMAGEIMP) $(QT_CPP11_FLAGS); $(MAKE)
else
	$(error Don't know how to make UIDIR = $(UIDIR))
endif


clean:
	-@for dir in $(ALLDIRS); do $(MAKE) -C $$dir $@; done
#	-@for dir in $(ALLDIRS); do ( cd $$dir; $(MAKE) $@ ); done
