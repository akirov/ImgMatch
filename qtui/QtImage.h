#ifndef __QTIMAGE_H__
#define __QTIMAGE_H__

#include <memory>

#include <QImage>

#include "Image.h"


class QtImage : public Image
{
  public:

    QtImage( std::string file_name );

    QtImage( const QtImage& other );

    ~QtImage();

    Image* Copy() const;

    inline int GetWidth() const { return mQImage->width(); }
    inline int GetHeight() const { return mQImage->height(); }

    std::string GetName() const { return mSrcFileURL; }  // inline is implicit

    Pixel GetPixel( int x, int y ) const;

    void Scale( int width, int height, AspectRatio aspect_ratio = Image::ASPR_IGNORE );


  private:

    std::string mSrcFileURL;
    std::auto_ptr<QImage> mQImage;
};


#endif /* __QTIMAGE_H__ */
