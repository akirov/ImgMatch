#-------------------------------------------------
#
# Project created by QtCreator 2010-05-08T00:22:46
#
#-------------------------------------------------

#CONFIG += debug
CONFIG += release
#CONFIG += static

#release:DESTDIR = ../$$(BINDIR)
#debug:DESTDIR = ../$$(BINDIR)
DESTDIR = ../$$(BINDIR)
OBJECTS_DIR = ../$$(BUILDDIR)
MOC_DIR = ../$$(BUILDDIR)


TARGET = imgmatch
TEMPLATE = app


#INCLUDEPATH += ../core
INCLUDEPATH += ../$$(COREDIR)

#LIBS += -L"../build" -limgmatch
LIBS += -L"../$$(BUILDDIR)" -limgmatch

#PRE_TARGETDEPS += ../build/libimgmatch.a
PRE_TARGETDEPS += ../$$(BUILDDIR)/libimgmatch.a

SOURCES += main.cpp \
           ImgMatchUI.cpp \
           Image.cpp

HEADERS += ImgMatchUI.h


#message (IMAGEIMP=$$(IMAGEIMP))

#contains (IMAGEIMP, QTIMAGE) {
QTIMAGE_RGB32|QTIMAGE_RGB24 {
    QTIMAGE_RGB32 {
        message (QTIMAGE_RGB32)
        DEFINES += QTIMAGE_RGB32
    } else:QTIMAGE_RGB24 {
        message (QTIMAGE_RGB24)
        DEFINES += QTIMAGE_RGB24
    }
    SOURCES += QtImage.cpp
    HEADERS += QtImage.h
}
else:OCVIMAGE_8UC3 {
    DEFINES += OCVIMAGE_8UC3
    SOURCES += OCVImage.cpp
    HEADERS += OCVImage.h
}
else {
    message (Unknown Image implementation)
}

USE_OPENCV = $$(USE_OPENCV)
contains (USE_OPENCV, 1) | OCVIMAGE_8UC3 {
    OPENCVINC = $$(OPENCVINC)
    OPENCVLIB = $$(OPENCVLIB)
    OPENCVVER = $$(OPENCVVER)

    !isEmpty(OPENCVINC) {
        INCLUDEPATH += $$(OPENCVINC)
    }

    !isEmpty(OPENCVLIB) {
        LIBS += -L$$(OPENCVLIB)
    }

    LIBS += -lopencv_core$$(OPENCVVER) -lopencv_highgui$$(OPENCVVER) \
            -lopencv_ml$$(OPENCVVER) -lopencv_imgproc$$(OPENCVVER)
}

FORMS += ImgMatchUI.ui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
