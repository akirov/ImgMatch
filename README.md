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

To compile the sources you need a C++11 compiler, CMake, Qt SDK, and optionally
OpenCV. You may need to specify OpenCV and Qt locations in root CMakeLists.txt,
if they are not standard. Then just `cd build`, `cmake .. -G "Unix Makefiles"`
(or use other generator), `make` or `mingw-make` and `make install`.
The executable file will be in `bin` sub-directory.


Screenshots
-----------

![first tab](https://github.com/akirov/ImgMatch/raw/master/docs/screen_1.jpg)

![second tab](https://github.com/akirov/ImgMatch/raw/master/docs/screen_2.jpg)

![third tab](https://github.com/akirov/ImgMatch/raw/master/docs/screen_3.jpg)
