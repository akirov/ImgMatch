#include "OCVImage.h"
#include "Logger.h"


#if defined(__GXX_EXPERIMENTAL_CXX0X) || __cplusplus >= 201103L
const std::map<Image::PixelRep, int> OCVImage::mPixelTypeMap = {
//      { Image::PIXEL_UNKNOWN, -1 },
        { Image::PIXEL_Gray8,   CV_8UC1},
        { Image::PIXEL_RGB24,   CV_8UC3},
        { Image::PIXEL_RGB32,   CV_8UC4},
//      { Image::PIXEL_ARGB32,  CV_8UC4}
};

#else  // No C++11

std::map<Image::PixelRep, int> OCVImage::createPixelTypeMap()
{
    std::map<Image::PixelRep, int> pixelTypeMap;
//  pixelFormatMap[Image::PIXEL_UNKNOWN] = -1;  // This is important (to be commented)!
    pixelTypeMap[Image::PIXEL_Gray8]     = CV_8UC1;
    pixelTypeMap[Image::PIXEL_RGB24]     = CV_8UC3;
    pixelTypeMap[Image::PIXEL_RGB32]     = CV_8UC4;
//  pixelTypeMap[Image::PIXEL_ARGB32]    = CV_8UC4;
    return pixelTypeMap;
}

const std::map<Image::PixelRep, int> OCVImage::mPixelTypeMap =
        OCVImage::createPixelTypeMap();
#endif  // C++11


Image::PixelRep OCVImage::getPixelRepFromType(int type)
{
    // No need for a reverse map...
    for( std::map<Image::PixelRep, int>::const_iterator it=mPixelTypeMap.begin();
         it!=mPixelTypeMap.end(); ++it)
    {
        if ( type == it->second )
            return it->first;
    }
    return Image::PIXEL_UNKNOWN;
}


OCVImage::OCVImage( const std::string& file_name, PixelRep pixel_rep ) :
        mSrcFileURL( file_name ),
        mPixelRep( pixel_rep ),
        mMatImage()
{
    switch( pixel_rep )
    {
        case Image::PIXEL_Gray8:
            mMatImage = cv::imread(file_name, CV_LOAD_IMAGE_GRAYSCALE);
            break;
        case Image::PIXEL_RGB24:
            mMatImage = cv::imread(file_name, CV_LOAD_IMAGE_COLOR);
            break;
        case Image::PIXEL_RGB32:
            mMatImage = cv::imread(file_name, CV_LOAD_IMAGE_ANYCOLOR);  // The same as CV_LOAD_IMAGE_UNCHANGED
            break;
        case Image::PIXEL_UNKNOWN:
        default:
            mMatImage = cv::imread(file_name, CV_LOAD_IMAGE_UNCHANGED);
            break;
    }

    if( !mMatImage.data ) {
        THROW( "Unable to open image file '" << file_name << "'" << " in "
               << pixel_rep << " representation!");
    }

    if ( Image::PIXEL_UNKNOWN == pixel_rep )
        mPixelRep = getPixelRepFromType(mMatImage.type());
    else
        Convert(pixel_rep);  // Will reset mPixelRep to pixel_rep, or set the actual rep.
}


OCVImage::OCVImage( const OCVImage& other )
{
    mSrcFileURL = other.mSrcFileURL;
    mPixelRep = other.mPixelRep;
    other.mMatImage.copyTo(mMatImage);  // Or use Mat::clone()?
}


OCVImage::~OCVImage()
{
    // cv::Mat content is deleted automatically
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


void OCVImage::Convert( Image::PixelRep newRep )
{
    try
    {
        int newType = mPixelTypeMap.at(newRep);  // Will throw with PIXEL_UNKNOWN!
        int oldType = mMatImage.type();
        if ( oldType != newType )
        {
            LOG("Converting OCVImage representation from " << (int)mPixelRep 
                << "(" << oldType << ") to " << (int)newRep 
                << "(" << newType << ")");

            int code=-1;
            int dstCn=0;
            switch( newType )
            {
                case CV_8UC1:
                {
                    if ( oldType == CV_8UC3 )
                    {
                        code = CV_RGB2GRAY;
                        dstCn = 1;
                    }
                    break;
                }
                default:
                    // throw? or return false?
                    break;
            }
            cv::Mat newImage;
            cvtColor(mMatImage, newImage, code, dstCn);
            mMatImage = newImage;
        }
        mPixelRep = newRep;  // mPixelRep is valid now.
    }
    catch ( const std::exception& e )  // Or "..." ?
    {
        LOG("Error converting OCVImage representation from " << (int)mPixelRep 
            << " to " << (int)newRep << ": " << e.what());
        mPixelRep = getPixelRepFromType(mMatImage.type());  // Set the actual rep.
    }
}
