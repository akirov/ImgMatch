#-------------------------------------------------
#
# Project created by QtCreator 2010-05-08T00:22:46
#
#-------------------------------------------------

#CONFIG += debug
CONFIG += release
#CONFIG += static

release:DESTDIR = release
debug:DESTDIR = debug


TARGET = imgmatch
TEMPLATE = app


#INCLUDEPATH += ../$$(CORE)
INCLUDEPATH += ../core

#LIBS += -L"../$$(CORE)" -limgmatch
LIBS += -L"../core" -limgmatch

#PRE_TARGETDEPS += ../$$(CORE)/$$(IMGMATCH_LIB)
PRE_TARGETDEPS += ../core/libimgmatch.a

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
    INCLUDEPATH += $$(OPENCVINC)
    LIBS += -L$$(OPENCVLIB) -lopencv_core$$(OPENCVVER) -lopencv_highgui$$(OPENCVVER) \
            -lopencv_ml$$(OPENCVVER) -lopencv_imgproc$$(OPENCVVER)
    SOURCES += OCVImage.cpp
    HEADERS += OCVImage.h
}
else {
    message (Unknown Image implementation)
}

FORMS += ImgMatchUI.ui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
