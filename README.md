ImgMatch
========

ImgMatch is a framework for image comparison and pattern recognition.

ImgMatch is free. It is licensed under LGPL.

It can be used to find similar images in a set or between two sets using
different, configurable algorithms.

Future plans include more matching algorithms implementation, benchmarking,
speed optimizations and more complex tasks like recognizing/searching faces,
shapes, fingerprints and text in images.

ImgMatch is written in C++, in an object-oriented and modular fashion.
It has a platform-independent GUI, which uses Qt framework.

To compile the source in Windows environment you need Cmake, Qt SDK and MinGW.
In Linux (or Cygwin) environment you need g++, qt-devel and cmake. If you
want to use OpenCV, you may need to specify its location in root CMakeLists.txt.
Then just `cd build`, `cmake .. -G "Unix Makefiles"`, `make` or `mingw-make` and
`make install`. The executable file will be in `bin` directory.


Screenshots
-----------

![first tab](https://github.com/akirov/ImgMatch/raw/master/docs/screen_1.jpg)

![second tab](https://github.com/akirov/ImgMatch/raw/master/docs/screen_2.jpg)

![third tab](https://github.com/akirov/ImgMatch/raw/master/docs/screen_3.jpg)
