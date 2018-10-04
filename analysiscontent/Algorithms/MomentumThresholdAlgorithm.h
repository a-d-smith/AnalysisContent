/**
 *  @file   analysiscontent/Algorithms/MomentumThresholdAlgorithm.h
 *
 *  @brief  Header file for the momentum threshold algorithm
 */

#ifndef MOMENTUM_THRESHOLD_ALGORITHM_H
#define MOMENTUM_THRESHOLD_ALGORITHM_H 1

#include "Pandora/Algorithm.h"

namespace analysis_content
{

class MomentumThresholdAlgorithm : public pandora::Algorithm
{
public:
    MomentumThresholdAlgorithm();

protected:
    pandora::StatusCode ReadSettings(const pandora::TiXmlHandle xmlHandle);
    pandora::StatusCode Run();
};

} // namespace analysis_content

#endif // MOMENTUM_THRESHOLD_ALGORITHM
