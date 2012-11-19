#include "ImgMatch.h"


class ModScale : public ImgMatch
{
  public:

    virtual double Compare( const Image& img1, const Image& img2 );

  private:

    static const int ScaledWidth = 64;
    static const double MaxDistance = 441.673;  // sqrt(3*255^2)

    // metrics ?
};
