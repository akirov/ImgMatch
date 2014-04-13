
COREDIR=core
UIDIR=qtui

# OpenCV instalation parameters
ifeq ($(OS),Windows_NT)
OPENCVLIB=D:/OpenCV/mingw/lib
OPENCVINC=D:/OpenCV/build/include
OPENCVVER=248
else
endif

# Whether to use Qt Image implementation or OpenCV. Uncomment one:
#IMAGEIMP=QTIMAGE_RGB32
IMAGEIMP=OCVIMAGE_8UC3
