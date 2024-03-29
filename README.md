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

To build the sources you need a C++11 compiler, CMake, Qt SDK, and optionally
OpenCV. You may need to specify OpenCV and Qt locations in root CMakeLists.txt,
if they are not standard. Then just `cd build`, `cmake .. -G "Unix Makefiles"`
(or for Visual Studio 2015 x64: `cmake .. -G "Visual Studio 14 2015" -A x64`),
then `make` or `mingw32-make` (or for MSVC: `devenv ImgMatch.sln /Build Release`).
After `make install` the executable file will be in `bin` sub-directory.


Screenshots
-----------

![first tab](https://github.com/akirov/ImgMatch/raw/master/docs/screen_1.jpg)

![second tab](https://github.com/akirov/ImgMatch/raw/master/docs/screen_2.jpg)

![third tab](https://github.com/akirov/ImgMatch/raw/master/docs/screen_3.jpg)


ToDo
----

- Deleting compare thread: pass parent QObject and verify automatic
  deletion, but not clear when this deletion will happen.
  Or we can delete the thread object manually in compareFinished() after
  waiting for it to exit. DONE.
  Or just connect &CompareThread::finished() with &QObject::deleteLater()?

- Extract CompareThread in a separate file? Rename it to CompareManagerThread?
  Then it can delegate each ImgMatch::Compare() to a pool of worker threads.
  Or we can have one thread to read the images (because HDD access is serial),
  giving them for parallel processing (calculate signature) to a pool of threads,
  then the first thread can compare signatures. In ImgMatch::Compare() we can
  pass a Signature abstract class ref, which will be implemented by each module.
  Pool of worker threads options: POSIX, QThreadPool, QtConcurrent, or C++11.

- Offer to save the cache of image signatures for later reuse, but the
  format will be different for different modules - need proper description.
  Add "Save" results button alongside "Clear"? May need to expand the form.

- "Delete" buttons working. Should we delete the entire row from the table?

- Add "Pause" button and display intermediate results?

- Provide access (void pointer or #ifdefs?) to Pixel implementation
  (cv::Mat or QImage), so native methods can be called.

- Change visualization of matching images: replace the two separate images with
  a single image consisting of two concatenated images with a separtor between
  them? Each matching class will need to provide a method to generate such
  combined image given two image URIs. This will give him the opportunity to
  draw lines between corresponding features for example! Provide default
  implementation.

- Implement SIFT / SURF / FAST / ORB features match using OpenCV:
  - put all these in a single box ("Features match") with options
  - use RANSAC to match feature points from two images
  - if we want to display matching points, first have to change the visualization
    as described above

- Implement histogram compare module:
  - use OpenCV's compareHist()
  - HOG + SVM option?

- ModScale improvements:
  - convert the images to grayscale (0-255) before resizing them? then we
    will have only one dimension; use cv::normalize()?
  - take the average of some key-regions (grid) instead of single points?
  - do K-means clustering by color first?
  - smooth (denoise) first?
  - rotate according to principle components first?
  - when one image is smaller than the thumbnail scale the bigger image to
    the smaller
  - metrics options:
    now we calculate the distance between the points in the RGB space and
    assume that the max distance is between white and black, then take the
    average;
    we can use |a-b|/(|a| + |b|) (euclidean)?;
    or use the angle between color vectors? but we have to move the origin
    at (128, 128, 128) may be
  - crop if the dimensions ratio is very different?
  - use OpenCV's matchTemplate()?

- Implement image signature module:
  - can use TensofFlow C++ library, or just cv::dnn::readNetFromTensorflow(),
    with a pre-trained in Pyhton neural network to create image signatures
    (from VGG-16 or similar, or can use transfer learning like
     https://blog.metaflow.fr/tensorflow-saving-restoring-and-mixing-multiple-models-c4c94d5d7125)
  - or use pre-trained YOLO for signatures?

- Shape matching module:
  - Elongatedness
  - Centroidal Profile
  - generalized Hough transform
  - PCA

- Face matching module with OpenCV:
  - add interface option to group similar faces in results?
  - Eigenfaces
  - Fisherfaces
  - Local Binary Patterns Histograms

- Rename PixelRGB to Pixel8UC3 (or PixelABC, or PixelXYZ), because we can
  have other 3 independent parameters (YUV, HLS, CMY...), not only RGB.
  Write conversion methods?

- Remove non-C++11 code, as C++03 is not supported any more.

- OpenCL/CUDA optimizations.
