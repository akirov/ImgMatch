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

//  OCVImage* Copy() const;

    inline int GetWidth() const { return mMatImage.size().width; }  // Or mMatImage.cols ?
    inline int GetHeight() const { return mMatImage.size().height; }  // Or mMatImage.rows ?

    std::string GetName() const { return mSrcFileURL; }  // inline is implicit

    PixelRGB GetPixelRGB( int x, int y ) const;

    void Scale( int width, int height, AspectRatio aspect_ratio = Image::ASPR_IGNORE );

    void Convert(Image::PixelRep newRep);

    cv::Mat* getImpl() { return &mMatImage; }  // If we want to use implementation specific methods. Unsafe!

  protected:

  private:

    std::string mSrcFileURL;
    Image::PixelRep mPixelRep;
    cv::Mat mMatImage;  // Or shared_ptr<IplImage> ?
};


#endif /* __OCVIMAGE_H__ */
