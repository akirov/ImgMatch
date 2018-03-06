
# Image implementation options: Qt or OpenCV. Uncomment only one.
#IMAGEIMP=OCVIMAGE_8UC3
IMAGEIMP=QTIMAGE_RGB24

# Components implementation options: "OPENCV" = use OpenCV, "0" = disabled.
SIFT_IMP=0

# Set OpenCV instalation parameters (if using OpenCV).
ifeq ($(OS),Windows_NT)
  ifeq (,$(findstring CYGWIN, $(shell uname)))
    # Windows, but not Cygwin
    #OPENCVLIB="D:/OpenCV/mingw/lib"
    #OPENCVINC="D:/OpenCV/build/include"
    #OPENCVVER=2413
    OPENCVLIB="D:/OpenCV/build/2.4.13.5/mingw/install/x86/mingw/lib"
    OPENCVINC="D:/OpenCV/build/2.4.13.5/mingw/install/include"
    OPENCVVER=2413
  endif
else
endif

# Whether to use C++11 standard. "1" = yes, "0" = no.
USE_CPP11=1

# Enable or disable logging. "1" = enabled. Log file is in bin directory by default.
ENABLE_LOG=1
