/**
 *  @file   analysiscontent/Algorithms/MomentumThresholdAlgorithm.h
 *
 *  @brief  Header file for the momentum threshold algorithm
 */

#ifndef MOMENTUM_THRESHOLD_ALGORITHM_H
#define MOMENTUM_THRESHOLD_ALGORITHM_H 1

#include "Pandora/Algorithm.h"

#include "larpandoracontent/LArHelpers/LArMCParticleHelper.h"

namespace analysis_content
{

class MomentumThresholdAlgorithm : public pandora::Algorithm
{
public:
    MomentumThresholdAlgorithm();
    ~MomentumThresholdAlgorithm();

protected:
    pandora::StatusCode ReadSettings(const pandora::TiXmlHandle xmlHandle);
    pandora::StatusCode Run();

private:
    typedef std::unordered_map<const pandora::MCParticle *, bool> MCParticleToBoolMap;
    typedef lar_content::LArMCParticleHelper LArMCParticleHelper;

    void GetNeutrinoFinalStates(const pandora::MCParticleList *pMCParticleList, const pandora::CaloHitList *pCaloHitList,
        pandora::MCParticleVector &finalStateParticles, LArMCParticleHelper::MCContributionMap &mcParticleToHitsMap) const;

    void IdentifyReconstructableMCParticles(const pandora::MCParticleList *pMCParticleList,	const pandora::CaloHitList *pCaloHitList,
        const pandora::MCParticleVector &finalStateParticles, MCParticleToBoolMap &mcParticleToIsReconstrutableMap) const;

    void GetOrderedMCParticleVector(const LArMCParticleHelper::MCContributionMap &mcParticleToHitsMap,
        pandora::MCParticleVector &finalStateParticles) const;

    int CountHits(const pandora::MCParticle *const pMCParticle, const LArMCParticleHelper::MCContributionMap &mcParticleToHitsMap,
        const pandora::HitType &view) const;

    std::string  m_caloHitListName;
    std::string  m_mcParticleListName;

    std::string  m_treeName;
    std::string  m_fileName;
};

} // namespace analysis_content1

#endif // MOMENTUM_THRESHOLD_ALGORITHM
