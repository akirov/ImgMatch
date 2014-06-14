#ifndef IMGMATCH_H
#define IMGMATCH_H

#if 0
#include "Image.h"
#else
#include <string>
#endif // 0


typedef enum
{
    MOD_INVALID=-1,
    MOD_SCALE,
    MOD_IMG_SIG,
    MOD_HISTOGRAM,
    MOD_SIFT,
    MOD_SURF,
    MOD_FACE,
    MOD_SHAPE,
    MOD_FINGERPRINT,
    MOD_TEXT
} MatchMode;


class ImgMatch
{
  public:

    virtual ~ImgMatch()
    {
    }

    /* Returns match value in the range [0.00, 1.00] */
#if 0  // Takes the images themselves as parameters
    virtual float Compare( const Image& img1, const Image& img2 ) = 0;
#else  // Takes image locations (paths)
    virtual float Compare( const std::string& img1path, const std::string& img2path ) = 0;
#endif // 0

  private:
    
};


#endif /* IMGMATCH_H */
