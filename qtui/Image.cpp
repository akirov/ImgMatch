#include "Image.h"
#if defined(QTIMAGE_RGB32) || defined(QTIMAGE_RGB24)
#include "QtImage.h"
#endif // QTIMAGE


Image* Image::ConstuctImage(std::string imageUrl)
{
#if defined(QTIMAGE_RGB32) || defined(QTIMAGE_RGB24)
    return new QtImage(imageUrl);
#else
#error "No Image implementation!"
#endif // IMAGEIMP
}
