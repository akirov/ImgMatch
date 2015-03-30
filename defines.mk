
# Whether to use Qt Image implementation or OpenCV. Uncomment only one:
#IMAGEIMP=QTIMAGE_RGB24
#IMAGEIMP=QTIMAGE_RGB32
IMAGEIMP=OCVIMAGE_8UC3

# Whether to link with OpenCV (1=Yes, 0=No). OCVIMAGE forces this.
USE_OPENCV=1

# OpenCV instalation parameters (if needed).
ifeq ($(OS),Windows_NT)
  ifeq (,$(findstring CYGWIN, $(shell uname)))
    # Windows, but not Cygwin
    OPENCVLIB="D:/OpenCV/mingw/lib"
    OPENCVINC="D:/OpenCV/build/include"
    OPENCVVER=248
  endif
else
endif
