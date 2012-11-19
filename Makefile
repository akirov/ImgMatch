
include ./defines.mk


#COREDIR = core

ALLDIRS = $(COREDIR) $(UIDIR)

.PHONY: $(ALLDIRS) all clean


all: $(COREDIR) $(UIDIR)


$(COREDIR):
	$(MAKE) -C $@


$(UIDIR):
ifeq (qtui, $(UIDIR))
	cd $(UIDIR); qmake CONFIG+=$(IMAGEIMP); $(MAKE)
#	cd $(UIDIR); qmake IMAGEIMP=$(IMAGEIMP); $(MAKE)
else
	$(error Unknown UIDIR = $(UIDIR))
endif


clean:
	-@for dir in $(ALLDIRS); do $(MAKE) -C $$dir $@; done
#	-@for dir in $(ALLDIRS); do ( cd $$dir; $(MAKE) $@ ); done
