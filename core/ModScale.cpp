#include <cstdlib>
#include <memory>
#include <cmath>

#include "ModScale.h"
#include "Logger.h"


ModScale::~ModScale()
{
    LOG("Deleting cache!");
    for ( std::map<std::string, Image*>::iterator i = mScaledImagesCache.begin();
          i != mScaledImagesCache.end(); ++i )
    {
        delete i->second;
    }
}


double ModScale::Compare( const std::string& img1path, const std::string& img2path )
{
    Image *scaledImg1, *scaledImg2;

    std::map<std::string, Image*>::iterator i = mScaledImagesCache.find(img1path);
    if ( i != mScaledImagesCache.end() )
    {
//        LOG("Image " << i->first << " found in cache!");
        scaledImg1 = i->second;
    }
    else
    {
//        LOG("Image " << img1path << " NOT found in cache!");
        scaledImg1 = Image::ConstuctImage(img1path);  // Check for errors when opening the images!
//        int ScaledHeight = scaledImg1->GetHeight() * ScaledWidth / scaledImg1->GetWidth();
        scaledImg1->Scale( ScaledWidth, ScaledHeight, Image::ASPR_IGNORE );  // Was Image::ASPR_KEEP
        mScaledImagesCache[img1path] = scaledImg1;
    }

    i = mScaledImagesCache.find(img2path);
    if ( i != mScaledImagesCache.end() )
    {
//        LOG("Image " << i->first << " found in cache!");
        scaledImg2 = i->second;
    }
    else
    {
//        LOG("Image " << img2path << " NOT found in cache!");
        scaledImg2 = Image::ConstuctImage(img2path);
        scaledImg2->Scale( ScaledWidth, ScaledHeight, Image::ASPR_IGNORE );
        mScaledImagesCache[img2path] = scaledImg2;
    }

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
//    LOG("img1=" << img1path << ", img2=" << img2path << ", distance=" << distance);

    return (1 - distance / MaxDistance);  // %
}
