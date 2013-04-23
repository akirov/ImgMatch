#-------------------------------------------------
#
# Project created by QtCreator 2010-05-08T00:22:46
#
#-------------------------------------------------

#CONFIG += debug

CONFIG += release
#CONFIG += static


TARGET = imgmatch
TEMPLATE = app


INCLUDEPATH += ../core

LIBS += -L"../core" -limgmatch

PRE_TARGETDEPS += ../core/libimgmatch.a

SOURCES += main.cpp \
           ImgMatchUI.cpp

HEADERS += ImgMatchUI.h


message (IMAGEIMP=$$(IMAGEIMP))

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
} else {
    message (IMAGEIMP is not implemented)
}

FORMS += ImgMatchUI.ui
