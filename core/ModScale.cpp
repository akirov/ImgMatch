#include <cstdlib>
#include <memory>
#include <cmath>

#include "ModScale.h"
#include "Logger.h"


double ModScale::Compare( const Image& img1, const Image& img2 )
{
    int ScaledHeight = img1.GetHeight() * ScaledWidth / img1.GetWidth();

    std::auto_ptr<Image> scaledImg1( img1.Copy() );
    scaledImg1->Scale( ScaledWidth, ScaledHeight, Image::ASPR_KEEP );  // Or Image::ASPR_IGNORE ?

    std::auto_ptr<Image> scaledImg2( img2.Copy() );
    scaledImg2->Scale( ScaledWidth, ScaledHeight, Image::ASPR_IGNORE );

    double distance=0;
    Pixel pixel1, pixel2;

    for( int x=0; x<ScaledWidth; x++ )
    {
        for( int y=0; y<ScaledHeight; y++ )
        {
            pixel1 = scaledImg1->GetPixel(x, y);

            pixel2 = scaledImg2->GetPixel(x, y);

            distance += ::sqrt((pixel1.R - pixel2.R)*(pixel1.R - pixel2.R) +
                               (pixel1.G - pixel2.G)*(pixel1.G - pixel2.G) +
                               (pixel1.B - pixel2.B)*(pixel1.B - pixel2.B));
        }
    }

    distance /= ScaledWidth*ScaledHeight;  // The average
    LOG("img1=" << img1.GetName() << ", img2=" << img2.GetName() << ", distance=" << distance);

    return (1 - distance / MaxDistance);  // %
}
