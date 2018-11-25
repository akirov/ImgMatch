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


ToDo
----

- use requestInterruption() and isInterruptionRequested(), or make thread
  stop flag atomic

- logger is not thread safe - add a lock

- handle closing the main window during processing, e. g.
  "Ignore closeEvent() during processing, only call MyThread::stop(), and
   then when QThread::finished() signal arrives, you can actually close the
   window."

- deleting compare thread: pass parent QObject and verify automatic
  deletion, but not clear when this deletion will happen;
  or we can delete the thread object manually in compareFinished() after
  waiting for it to exit (where?);
  or just connect &CompareThread::finished() with &QObject::deleteLater()

- remove non-C++11 code, if C++03 support can't be guaranteed?

- rename PixelRGB to Pixel8UC3 (or PixelABC, or PixelXYZ), because we can
  have other 3 independent parameters (YUV, HLS, CMY...), not only RGB;
  write conversion methods;

- provide access (void pointer or #ifdefs?) to Pixel implementation
  (cv::Mat or QImage), so native methods can be called (with #ifdefs)?

- process files in parallel with a pool of threads - POSIX (see Wav2mp3),
  QThreadPool, or QtConcurrent

- we don't need to store image names in the results, we can store image
  index in an array of names (URIs) only, so change ComPair?

- offer to save the cache of image signatures for later reuse, but the
  format will be different for different modules - need proper description;
  add "Save" results button alongside "Clear"? will need to expand the form

- if we want to modify the image e. g. to display matching points, think
  how to do it - where and how to store processed images

- "Delete" buttons working; should we delete the entire row from the table?

- ModScale improvements:

  convert the images to grayscale (0-255) before resizing them? then we
  will have only one dimension

  take the average of some key-regions (grid) instead of single points?

  smooth (denoise) first?

  rotate according to principle components first?

  find the "most interesting region" first?

  when one image is smaller than the thumbnail scale the bigger image to
  the smaller

  metrics options:
  now we calculate the distance between the points in the RGB space and
  assume that the max distance is between white and black, then take the
  average;
  we can use |a-b|/(|a| + |b|) (euclidean)?;
  or use the angle between color vectors? but we have to move the origin
  at (128, 128, 128) may be

  crop if the dimensions ratio is very different?

- implement SIFT / SURF / FAST / ORB modules using OpenCV; need to call a
  decoration function to add matching points before displaying the images

- implement histogram compare module

- implement image signature module:
  can use TensofFlow C++ library with a pre-trained in Pyhton neural network
  to create image signatures;
  or use OpenCV ML methods

- implement face matching module

- OpenCL/CUDA optimizations
