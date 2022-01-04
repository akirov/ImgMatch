#include "ModOcvFeat.h"


ModOcvFeat::ModOcvFeat(ModOcvFeat::OcvFeatType feature)
{
    switch( feature )
    {
        case OcvFeatType::OCV_FEAT_SIFT:
        {
            mDetector = std::make_unique<SIFTFeatureDetector>();
            break;
        }
        default:
        {
            // throw?
            break;
        }
    };
}
