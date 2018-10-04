/**
 *  @file   larphysicscontent/AnalysisContent.h
 *
 *  @brief  Header file detailing content for use with physics analyses.
 *
 *  $Log: $
 */
#ifndef ANALYSIS_CONTENT_H
#define ANALYSIS_CONTENT_H

namespace pandora
{
class Pandora;
}

/**
 *  @brief  AnalysisContent class
 */
class AnalysisContent
{
public:
    /**
     *  @brief  Register all the lar content algorithms and tools with pandora
     *
     *  @param  pandora the pandora instance with which to register content
     */
    static pandora::StatusCode RegisterAlgorithms(const pandora::Pandora &pandora);
};

#endif // #ifndef ANALYSIS_CONTENT_H
