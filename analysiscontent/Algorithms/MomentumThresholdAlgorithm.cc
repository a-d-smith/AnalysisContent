/**
 *  @file   analysiscontent/Algorithms/MomentumThresholdAlgorithm.cc
 *
 *  @brief  Implementation file for the momentum threshold algorithm
 */

#include "analysiscontent/Algorithms/MomentumThresholdAlgorithm.h"

#include "Pandora/AlgorithmHeaders.h"

#include <iostream>

using namespace pandora;

namespace analysis_content
{

MomentumThresholdAlgorithm::MomentumThresholdAlgorithm()
{
    std::cout << "I have been created!" << std::endl;
}

//------------------------------------------------------------------------------------------------------------------------------------------

StatusCode MomentumThresholdAlgorithm::Run()
{
    return STATUS_CODE_SUCCESS;
}

//------------------------------------------------------------------------------------------------------------------------------------------

StatusCode MomentumThresholdAlgorithm::ReadSettings(const TiXmlHandle /*xmlHandle*/)
{
    return STATUS_CODE_SUCCESS;
}

} // namespace analysis_content
