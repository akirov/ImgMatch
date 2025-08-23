#include "ModOcvFeat.h"


ModOcvFeat::ModOcvFeat(ModOcvFeat::OcvFeatType feature)
{
    switch( feature )
    {
        case OcvFeatType::OCV_FEAT_SIFT:
        {
#if (CV_MAJOR_VERSION < 4)
            mDetector = std::make_unique<SIFTFeatureDetector>();
#else
            mDetector = cv::SIFT::create();
#endif  // CV_MAJOR_VERSION
            break;
        }
        default:
        {
            // throw?
            break;
        }
    };
}
