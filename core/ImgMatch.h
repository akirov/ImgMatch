#ifndef IMGMATCH_H
#define IMGMATCH_H

#include "Image.h"


typedef enum
{
    MOD_INVALID=-1,
    MOD_SCALE_DOWN,
    MOD_IMG_SIG,
    MOD_2D_FFT,
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
    virtual double Compare( const Image& img1, const Image& img2 ) = 0;

  private:
    
};


#endif /* IMGMATCH_H */
