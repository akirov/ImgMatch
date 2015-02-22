#include "OCVImage.h"
#include "Logger.h"


OCVImage::OCVImage( const std::string& file_name ) :
        mSrcFileURL( file_name ),
        mMatImage( cv::imread(file_name, CV_LOAD_IMAGE_COLOR) )
{
    if( !mMatImage.data ) {
        THROW( "Unable to open image file '" << file_name << "'" );
    }
}


OCVImage::OCVImage( const OCVImage& other )
{
    mSrcFileURL = other.mSrcFileURL;
    other.mMatImage.copyTo(mMatImage);  // Or use Mat::clone()?
}


OCVImage::~OCVImage()
{
    // cv::Mat is deleted automatically
}


PixelRGB OCVImage::GetPixelRGB( int x, int y ) const
{
    cv::Vec3b data = mMatImage.at<cv::Vec3b>(y,x);  // Because it is at(row, column)
    return PixelRGB(data[2], data[1], data[0]);  // 0-B, 1-G, 2-R
}


void OCVImage::Scale( int width, int height, AspectRatio aspect_ratio )
{
    cv::Mat resizedImage;  // Do we need this, or src and dest can be the same?

    if ( aspect_ratio == ASPR_IGNORE )
        cv::resize(mMatImage, resizedImage, cv::Size(width, height)); // , 0, 0, cv::INTER_AREA
    else
        cv::resize(mMatImage, resizedImage, cv::Size(width,
                   mMatImage.size().height * (width / mMatImage.size().width)));

    mMatImage = resizedImage;  // Frees the old data.
}
