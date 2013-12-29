#include <map>
#include <string>
#include "ImgMatch.h"
#include "Image.h"


class ModScale : public ImgMatch
{
  public:

    ~ModScale();

    virtual double Compare( const std::string& img1path, const std::string& img2path );

  private:

    static const int ScaledWidth = 15;
    static const int ScaledHeight = 10;  // ~3:2
    static const double MaxDistance = 441.673;  // sqrt(3*255^2), Euclidean!

    std::map<std::string, Image*> mScaledImagesCache;  // Or shared_ptr<Image> ?

    // metrics ?
};
