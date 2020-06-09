#include "QtImage.h"
#include "Logger.h"

#if defined(__GXX_EXPERIMENTAL_CXX0X) || __cplusplus >= 201103L || (defined(_MSC_VER) && _MSC_VER >= 1900)
const std::map<Image::PixelRep, QImage::Format> QtImage::mPixelFormatMap = {
//      { Image::PIXEL_UNKNOWN, QImage::Format_Invalid },
        { Image::PIXEL_Gray8,   QImage::Format_Indexed8},
        { Image::PIXEL_RGB24,   QImage::Format_RGB888},
        { Image::PIXEL_RGB32,   QImage::Format_RGB32},
        { Image::PIXEL_ARGB32,  QImage::Format_ARGB32}
};

#else  // No C++11

std::map<Image::PixelRep, QImage::Format> QtImage::createPixelFormatMap()
{
    std::map<Image::PixelRep, QImage::Format> pixelFormatMap;
//  pixelFormatMap[Image::PIXEL_UNKNOWN] = QImage::Format_Invalid;  // This is important (to be commented)!
    pixelFormatMap[Image::PIXEL_Gray8]   = QImage::Format_Indexed8;
    pixelFormatMap[Image::PIXEL_RGB24]   = QImage::Format_RGB888;
    pixelFormatMap[Image::PIXEL_RGB32]   = QImage::Format_RGB32;
    pixelFormatMap[Image::PIXEL_ARGB32]  = QImage::Format_ARGB32;
    return pixelFormatMap;
}

const std::map<Image::PixelRep, QImage::Format> QtImage::mPixelFormatMap =
        QtImage::createPixelFormatMap();
#endif  // C++11


QVector<QRgb> QtImage::createGrayIndexTable()
{
    QVector<QRgb> table;
    for ( int i = 0; i < 256; ++i )
        table.push_back( qRgb( i, i, i ) );
    return table;
}

const QVector<QRgb> QtImage::mGrayIndexTable = QtImage::createGrayIndexTable();


Image::PixelRep QtImage::getPixelRepFromFormat(QImage::Format format)
{
    // No need for a reverse map...
    for( std::map<Image::PixelRep, QImage::Format>::const_iterator it=mPixelFormatMap.begin();
         it!=mPixelFormatMap.end(); ++it)
    {
        if ( format == it->second )
            return it->first;
    }
    return Image::PIXEL_UNKNOWN;
}


QtImage::QtImage( std::string file_name, PixelRep pixel_rep ) :
        mSrcFileURL( file_name ),
        mPixelRep(pixel_rep),
        mQImage( new QImage(file_name.c_str()) )  // Default format is Format_RGB32
{
    if( mQImage->isNull() ) {
        THROW( "Unable to open image file '" << file_name << "'" );
    }

    if ( PIXEL_UNKNOWN == pixel_rep )
        mPixelRep = getPixelRepFromFormat(mQImage->format());
    else
        Convert(pixel_rep);  // Will reset mPixelRep to pixel_rep, or set the actual rep.
}


QtImage::~QtImage()
{
    // shared_ptr is deleted automatically
}


QtImage::QtImage( const QtImage& other )
{
    mSrcFileURL = other.mSrcFileURL;
    mPixelRep = other.mPixelRep;
#if 0
    mQImage.reset(new QImage(*other.mQImage));
#else
    *mQImage = *other.mQImage;
#endif // 0
}


QtImage* QtImage::Copy() const
{
    return new QtImage(*this);
}


PixelRGB QtImage::GetPixelRGB( int x, int y ) const
{
    QRgb qt_argb;

    if ( mQImage->format() == QImage::Format_Indexed8)
        qt_argb = mQImage->color(mQImage->pixelIndex(x, y));
    else
        qt_argb = mQImage->pixel( x, y );  // Or try to get the raw data?

    return PixelRGB(qRed(qt_argb), qGreen(qt_argb), qBlue(qt_argb));
}


unsigned char QtImage::GetPixelGray( int x, int y ) const
{
    return (unsigned char) mQImage->pixelIndex(x, y);  // Index in GrayIndexTable = gray level.
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
    *mQImage = mQImage->scaled( width, height, qt_aspr );  // , Qt::SmoothTransformation
#endif /* 0 */
}


void QtImage::Convert(PixelRep newRep)
{
//  if ( newRep == mPixelRep ) return;  // mPixelRep may be invalid!

    try
    {
        QImage::Format newFormat = mPixelFormatMap.at(newRep);  // Will throw with PIXEL_UNKNOWN!
        if ( mQImage->format() != newFormat )
        {
            LOG("Converting QtImage representation from " << (int)mPixelRep 
                << "(" << (int)mQImage->format() << ") to " << (int)newRep 
                << "(" << (int)newFormat << ")");

            if ( newFormat == QImage::Format_Indexed8)
                *mQImage = mQImage->convertToFormat(newFormat, mGrayIndexTable);
            else
                *mQImage = mQImage->convertToFormat(newFormat);
        }
        mPixelRep = newRep;  // mPixelRep is valid now.
    }
    catch ( const std::exception& e )  // Or "..." ?
    {
        LOG("Error converting QtImage representation from " << (int)mPixelRep 
            << " to " << (int)newRep << ": " << e.what());
        mPixelRep = getPixelRepFromFormat(mQImage->format());  // Set the actual rep.
    }
}
