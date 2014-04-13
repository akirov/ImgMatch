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


Pixel OCVImage::GetPixel( int x, int y ) const
{
    return Pixel(mMatImage.at<cv::Vec3b>(y,x)[2], 
                 mMatImage.at<cv::Vec3b>(y,x)[1],
                 mMatImage.at<cv::Vec3b>(y,x)[0]);
}


void OCVImage::Scale( int width, int height, AspectRatio aspect_ratio )
{
	if ( aspect_ratio == ASPR_IGNORE )
		cv::resize(mMatImage, mMatImage, cv::Size(height, width));  // Or cv::resize(mMatImage, tmpImg, ...) ?
	else
		cv::resize(mMatImage, mMatImage, cv::Size(mMatImage.size().height *
				                         (width / mMatImage.size().width), width));
}
