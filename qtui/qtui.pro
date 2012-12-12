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


#contains (IMAGEIMP, QTIMAGE) { 
QTIMAGE { 
    message ("IMAGEIMP=QTIMAGE")
    SOURCES += QtImage.cpp
    HEADERS += QtImage.h
} else { 
    message ("IMAGEIMP=...") 
}

FORMS += ImgMatchUI.ui
