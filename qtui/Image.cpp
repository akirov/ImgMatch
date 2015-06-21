#include "Image.h"
#if defined(QTIMAGE_RGB32) || defined(QTIMAGE_RGB24)
#include "QtImage.h"
#elif defined(OCVIMAGE_8UC3)
#include "OCVImage.h"
#endif // QTIMAGE


Image* Image::ConstuctImage(std::string image_url, PixelRep pixel_rep)
{
#if defined(QTIMAGE_RGB32) || defined(QTIMAGE_RGB24)
    return new QtImage(image_url, pixel_rep);
#elif defined(OCVIMAGE_8UC3)
    return new OCVImage(image_url, pixel_rep);
#else
#error "No Image implementation!"
#endif // IMAGEIMP
}
