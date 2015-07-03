
# Image implementation options: Qt or OpenCV. Uncomment only one.
#IMAGEIMP=OCVIMAGE_8UC3
IMAGEIMP=QTIMAGE_RGB24

# Components implementation options: "OPENCV" = use OpenCV, "0" = disabled.
SIFT_IMP=0

# Set OpenCV instalation parameters (if using OpenCV).
ifeq ($(OS),Windows_NT)
  ifeq (,$(findstring CYGWIN, $(shell uname)))
    # Windows, but not Cygwin
    OPENCVLIB="D:/OpenCV/mingw/lib"
    OPENCVINC="D:/OpenCV/build/include"
    OPENCVVER=248
  endif
else
endif

# Whether to use C++11 standard. "1" = yes, "0" = no.
USE_CPP11=0
