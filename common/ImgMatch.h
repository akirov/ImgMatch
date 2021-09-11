#ifndef __IMGMATCH_H__
#define __IMGMATCH_H__

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
    MOD_FEATURES,
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

    /* Returns match value in the range [0.00, 1.00]. Negative value means error. */
#if 0  // Takes the images themselves as parameters
    virtual float Compare( const Image& img1, const Image& img2 ) = 0;
    // Or it can take a Signature abstract class ref, which will be implemented by each module...
#else  // Takes image locations (paths)
    virtual float Compare( const std::string& img1path, const std::string& img2path ) = 0;
#endif // 0

  private:
    
};


#endif /* __IMGMATCH_H__ */
