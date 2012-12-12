#include "QtImage.h"
#include "Logger.h"


QtImage::QtImage( std::string file_name ) :
        mSrcFileURL( file_name ),
        mQImage( new QImage(file_name.c_str()) )
{
    if( mQImage->isNull() ) {
        THROW( "Unable to open image file '" << file_name << "'" );
    }

    if ( mQImage->format() != QImage::Format_RGB32 ) {
        *mQImage = mQImage->convertToFormat(QImage::Format_RGB32);  // Or QImage::Format_ARGB32 ???
    }
}


QtImage::~QtImage()
{
    // auto_ptr is deleted automatically
}


QtImage::QtImage( const QtImage& other )
{
    mSrcFileURL = other.mSrcFileURL;
    mQImage.reset(new QImage(*other.mQImage));
}


Image* QtImage::Copy() const
{
    return new QtImage(*this);
}


Pixel QtImage::GetPixel( int x, int y ) const
{
    QRgb qt_argb = mQImage->pixel( x, y );  // Or try to get the raw data?
#if 1
    return Pixel(qRed(qt_argb), qGreen(qt_argb), qBlue(qt_argb));
#else  // Doesn't work!
    return Pixel(qt_argb);
#endif // 0/1
}


void QtImage::Scale(int width, int height, AspectRatio aspect_ratio)
{
    Qt::AspectRatioMode qt_aspr;
    switch( aspect_ratio )
    {
        case Image::ASPR_KEEP :
        {
            qt_aspr = Qt::KeepAspectRatio;
            break;
        }
        case Image::ASPR_IGNORE :
        {
            qt_aspr = Qt::IgnoreAspectRatio;
            break;
        }
        default :
        {
            THROW("Unrecognized aspect ratio.");
        }
    }
#if 0
    QImage scaled_image = mQImage->scaled( width, height, qt_aspr );
    mQImage.reset( new QImage(scaled_image) );
#else
    *mQImage = mQImage->scaled( width, height, qt_aspr );
#endif /* 0 */
}
