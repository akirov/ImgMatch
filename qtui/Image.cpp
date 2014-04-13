#include "Image.h"
#if defined(QTIMAGE_RGB32) || defined(QTIMAGE_RGB24)
#include "QtImage.h"
#elif defined(OCVIMAGE_8UC3)
#include "OCVImage.h"
#endif // QTIMAGE


Image* Image::ConstuctImage(std::string imageUrl)
{
#if defined(QTIMAGE_RGB32) || defined(QTIMAGE_RGB24)
    return new QtImage(imageUrl);
#elif defined(OCVIMAGE_8UC3)
    return new OCVImage(imageUrl);
#else
#error "No Image implementation!"
#endif // IMAGEIMP
}
