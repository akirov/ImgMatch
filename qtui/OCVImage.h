#ifndef __OCVIMAGE_H__
#define __OCVIMAGE_H__

#include <opencv2/opencv.hpp>

#include "Image.h"


class OCVImage : public Image
{
  public:

    OCVImage( const std::string& file_name );
    OCVImage( const OCVImage& other );

    ~OCVImage();

//  OCVImage* Copy() const;

    inline int GetWidth() const { return mMatImage.size().width; }  // Or mMatImage.cols ?
    inline int GetHeight() const { return mMatImage.size().height; }  // Or mMatImage.rows ?

    std::string GetName() const { return mSrcFileURL; }  // inline is implicit

    Pixel GetPixel( int x, int y ) const;

    void Scale( int width, int height, AspectRatio aspect_ratio = Image::ASPR_IGNORE );

  protected:

  private:

    std::string mSrcFileURL;
    cv::Mat mMatImage;  // Or IplImage* ?
};


#endif /* __OCVIMAGE_H__ */
