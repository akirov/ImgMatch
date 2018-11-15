#ifndef __QTIMAGE_H__
#define __QTIMAGE_H__

#include <map>
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
#include <QVector>

#include "Image.h"


class QtImage : public Image
{
  public:

    QtImage( std::string file_name, PixelRep pixel_rep );
    QtImage( const QtImage& other );

    ~QtImage();

    QtImage* Copy() const;

    inline int GetWidth() const { return mQImage->width(); }
    inline int GetHeight() const { return mQImage->height(); }

    std::string GetName() const { return mSrcFileURL; }  // inline is implicit

    inline PixelRGB GetPixelRGB( int x, int y ) const;
    inline unsigned char GetPixelGray( int x, int y ) const;

    void Scale( int width, int height, AspectRatio aspect_ratio = Image::ASPR_IGNORE );

    void Convert(PixelRep newRep);

  protected:

  private:

    static const std::map<Image::PixelRep, QImage::Format> mPixelFormatMap;
    static const QVector<QRgb>  mGrayIndexTable;

    static std::map<Image::PixelRep, QImage::Format> createPixelFormatMap();  // If not C++11
    static QVector<QRgb> createGrayIndexTable();
    static Image::PixelRep getPixelRepFromFormat(QImage::Format);

    std::string mSrcFileURL;  // Or URI?
    Image::PixelRep mPixelRep;
    shared_ptr<QImage> mQImage;
};


#endif /* __QTIMAGE_H__ */
