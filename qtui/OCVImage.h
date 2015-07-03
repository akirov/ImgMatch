#ifndef __OCVIMAGE_H__
#define __OCVIMAGE_H__

#include <opencv2/opencv.hpp>

#include "Image.h"


class OCVImage : public Image
{
  public:

    OCVImage( const std::string& file_name, PixelRep pixel_rep );
    OCVImage( const OCVImage& other );

    ~OCVImage();

//  OCVImage* Copy() const;  // Note: cv::Mat copy operators will only copy the headers and the pointer to the large matrix, not the data itself.

    inline int GetWidth() const { return mMatImage.size().width; }  // Or mMatImage.cols ?
    inline int GetHeight() const { return mMatImage.size().height; }  // Or mMatImage.rows ?

    std::string GetName() const { return mSrcFileURL; }  // inline is implicit

    inline PixelRGB GetPixelRGB( int x, int y ) const;
//  inline unsigned char GetPixelGray( int x, int y ) const;

    void Scale( int width, int height, AspectRatio aspect_ratio = Image::ASPR_IGNORE );

    void Convert(Image::PixelRep newRep);

    cv::Mat* getImpl() { return &mMatImage; }  // If we want to use implementation specific methods. Unsafe!

  protected:

  private:

    static Image::PixelRep getPixelRepFromFormat(int);

    std::string mSrcFileURL;
    Image::PixelRep mPixelRep;
    cv::Mat mMatImage;  // No need of pointers here.
};


#endif /* __OCVIMAGE_H__ */
