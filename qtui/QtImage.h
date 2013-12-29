#ifndef __QTIMAGE_H__
#define __QTIMAGE_H__

#if defined(__GXX_EXPERIMENTAL_CXX0X) || __cplusplus >= 201103L
  #include <memory>
  using std::shared_ptr;
  using std::unique_ptr;
#else
  #include <tr1/memory>
  using std::tr1::shared_ptr;
  // unique_ptr is NOT in tr1, it REPLACES auto_ptr in C++11
#endif

#include <QImage>

#include "Image.h"


class QtImage : public Image
{
  public:

    QtImage( std::string file_name );
    QtImage( const QtImage& other );

    ~QtImage();

    QtImage* Copy() const;

    inline int GetWidth() const { return mQImage->width(); }
    inline int GetHeight() const { return mQImage->height(); }

    std::string GetName() const { return mSrcFileURL; }  // inline is implicit

    Pixel GetPixel( int x, int y ) const;

    void Scale( int width, int height, AspectRatio aspect_ratio = Image::ASPR_IGNORE );

  protected:

  private:

    std::string mSrcFileURL;  // Or URI?
    shared_ptr<QImage> mQImage;  // Was std::auto_ptr
};


#endif /* __QTIMAGE_H__ */
