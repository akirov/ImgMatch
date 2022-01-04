#ifndef __MODOCVFEAT_H__
#define __MODOCVFEAT_H__

#include <map>
#include <memory>
#include "ImgMatch.h"
#include "Image.h"
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#if (CV_MAJOR_VERSION < 3)
 #include <opencv2/nonfree/features2d.hpp>
 #include <opencv2/nonfree/nonfree.hpp>
 typedef cv::SiftFeatureDetector SIFTFeatureDetector;
#else
 #include <opencv2/features2d.hpp>
 #include <opencv2/xfeatures2d/nonfree.hpp>
 typedef cv::xfeatures2d::SiftFeatureDetector SIFTFeatureDetector;
#endif  // CV_MAJOR_VERSION


class ModOcvFeat : public ImgMatch
{
  public:

     enum class OcvFeatType : uint8_t
     {
         OCV_FEAT_SIFT=0,
         OCV_FEAT_SURF
     };

     ModOcvFeat(OcvFeatType feature);
     ~ModOcvFeat();

    virtual float Compare( const std::string& img1path, const std::string& img2path );

  private:

    std::unique_ptr<cv::Feature2D> mDetector = nullptr;
    std::map<std::string, cv::Mat> mDescriptorsCache;
};


#endif // __MODOCVFEAT_H__
