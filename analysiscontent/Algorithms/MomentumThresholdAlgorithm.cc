/**
 *  @file   analysiscontent/Algorithms/MomentumThresholdAlgorithm.cc
 *
 *  @brief  Implementation file for the momentum threshold algorithm
 */

#include "analysiscontent/Algorithms/MomentumThresholdAlgorithm.h"

#include "Pandora/AlgorithmHeaders.h"
#include "Pandora/PdgTable.h"

#include "larpandoracontent/LArHelpers/LArMCParticleHelper.h"

#include <iostream>

using namespace pandora;

namespace analysis_content
{

MomentumThresholdAlgorithm::MomentumThresholdAlgorithm()
{
}

//------------------------------------------------------------------------------------------------------------------------------------------

MomentumThresholdAlgorithm::~MomentumThresholdAlgorithm()
{
    PANDORA_MONITORING_API(SaveTree(this->GetPandora(), m_treeName.c_str(), m_fileName.c_str(), "UPDATE"));
}

//------------------------------------------------------------------------------------------------------------------------------------------

StatusCode MomentumThresholdAlgorithm::Run()
{
    // Get the input collections
	const MCParticleList *pMCParticleList = nullptr;
    PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::GetList(*this, m_mcParticleListName, pMCParticleList));

	const CaloHitList *pCaloHitList = nullptr;
    PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::GetList(*this, m_caloHitListName, pCaloHitList));

    // Get the neutrino final state MCParticles
	LArMCParticleHelper::MCContributionMap mcParticleToHitsMap;
	MCParticleVector finalStateParticles;
	this->GetNeutrinoFinalStates(pMCParticleList, pCaloHitList, finalStateParticles, mcParticleToHitsMap);

    // Determine which are reconstructable
	MCParticleToBoolMap mcParticleToIsReconstrutableMap;
	this->IdentifyReconstructableMCParticles(pMCParticleList, pCaloHitList, finalStateParticles, mcParticleToIsReconstrutableMap);

    // Output the details to a tree
    for (const auto &pMCParticle : finalStateParticles)
    {
        PANDORA_MONITORING_API(SetTreeVariable(this->GetPandora(), m_treeName.c_str(), "pdg", pMCParticle->GetParticleId()));
        PANDORA_MONITORING_API(SetTreeVariable(this->GetPandora(), m_treeName.c_str(), "momentum", pMCParticle->GetMomentum().GetMagnitude()));
        PANDORA_MONITORING_API(SetTreeVariable(this->GetPandora(), m_treeName.c_str(), "energy", pMCParticle->GetEnergy()));
        PANDORA_MONITORING_API(SetTreeVariable(this->GetPandora(), m_treeName.c_str(), "mass", PdgTable::GetParticleMass(pMCParticle->GetParticleId())));
        PANDORA_MONITORING_API(SetTreeVariable(this->GetPandora(), m_treeName.c_str(), "nHitsU", this->CountHits(pMCParticle, mcParticleToHitsMap, TPC_VIEW_U)));
        PANDORA_MONITORING_API(SetTreeVariable(this->GetPandora(), m_treeName.c_str(), "nHitsV", this->CountHits(pMCParticle, mcParticleToHitsMap, TPC_VIEW_V)));
        PANDORA_MONITORING_API(SetTreeVariable(this->GetPandora(), m_treeName.c_str(), "nHitsW", this->CountHits(pMCParticle, mcParticleToHitsMap, TPC_VIEW_W)));

        const auto it = mcParticleToIsReconstrutableMap.find(pMCParticle);
        if (it == mcParticleToIsReconstrutableMap.end())
            throw StatusCodeException(STATUS_CODE_NOT_FOUND);

        PANDORA_MONITORING_API(SetTreeVariable(this->GetPandora(), m_treeName.c_str(), "isReconstructable", static_cast<int>(it->second)));
        PANDORA_MONITORING_API(FillTree(this->GetPandora(), m_treeName.c_str()));
    }

    return STATUS_CODE_SUCCESS;
}

//------------------------------------------------------------------------------------------------------------------------------------------

void MomentumThresholdAlgorithm::GetNeutrinoFinalStates(const MCParticleList *pMCParticleList, const CaloHitList *pCaloHitList, 
    MCParticleVector &finalStateParticles, LArMCParticleHelper::MCContributionMap &mcParticleToHitsMap) const
{
	if (!finalStateParticles.empty())
		throw StatusCodeException(STATUS_CODE_INVALID_PARAMETER);
	
	if (!mcParticleToHitsMap.empty())
		throw StatusCodeException(STATUS_CODE_INVALID_PARAMETER);

    // Get the MCParticles which have associated hits, by using existing validation mechanics
	LArMCParticleHelper::PrimaryParameters paramsAll;
	paramsAll.m_minPrimaryGoodHits = 0;
	paramsAll.m_minHitsForGoodView = 0;
	paramsAll.m_minHitSharingFraction = 0.f;
	LArMCParticleHelper::SelectReconstructableMCParticles(pMCParticleList, pCaloHitList, paramsAll, LArMCParticleHelper::IsBeamNeutrinoFinalState, mcParticleToHitsMap);

    // Also add in the MCParticles without hits
    for (const MCParticle *const pMCParticle : *pMCParticleList)
    {
        if (!LArMCParticleHelper::IsBeamNeutrinoFinalState(pMCParticle))
            continue;

        if (mcParticleToHitsMap.find(pMCParticle) != mcParticleToHitsMap.end())
            continue;

        mcParticleToHitsMap.emplace(pMCParticle, CaloHitList());
    }
	
    this->GetOrderedMCParticleVector(mcParticleToHitsMap, finalStateParticles);
}

//------------------------------------------------------------------------------------------------------------------------------------------

void MomentumThresholdAlgorithm::IdentifyReconstructableMCParticles(const MCParticleList *pMCParticleList, const CaloHitList *pCaloHitList,
    const MCParticleVector &finalStateParticles, MCParticleToBoolMap &mcParticleToIsReconstrutableMap) const
{
	if (!mcParticleToIsReconstrutableMap.empty())
		throw StatusCodeException(STATUS_CODE_INVALID_PARAMETER);

	LArMCParticleHelper::PrimaryParameters params;
	LArMCParticleHelper::MCContributionMap reconstructableMCParticleToHitsMap;
	LArMCParticleHelper::SelectReconstructableMCParticles(pMCParticleList, pCaloHitList, params, LArMCParticleHelper::IsBeamNeutrinoFinalState, reconstructableMCParticleToHitsMap);

	for (const auto &pMCParticle : finalStateParticles)
		mcParticleToIsReconstrutableMap.emplace(pMCParticle, reconstructableMCParticleToHitsMap.find(pMCParticle) != reconstructableMCParticleToHitsMap.end());
}

//------------------------------------------------------------------------------------------------------------------------------------------

void MomentumThresholdAlgorithm::GetOrderedMCParticleVector(const LArMCParticleHelper::MCContributionMap &mcParticleToHitsMap,
	MCParticleVector &finalStateParticles) const
{
	for (const auto &entry : mcParticleToHitsMap)
		finalStateParticles.push_back(entry.first);

	// Sort using the built in < operator
	std::sort(finalStateParticles.begin(), finalStateParticles.end());
}

//------------------------------------------------------------------------------------------------------------------------------------------

int MomentumThresholdAlgorithm::CountHits(const MCParticle *const pMCParticle,
    const LArMCParticleHelper::MCContributionMap &mcParticleToHitsMap, const HitType &view) const
{
    const auto it = mcParticleToHitsMap.find(pMCParticle);
    if (it == mcParticleToHitsMap.end())
        throw StatusCodeException(STATUS_CODE_NOT_FOUND);

    int nHits = 0;
    for (const auto &pCaloHit : it->second)
        nHits += (pCaloHit->GetHitType() == view) ? 1 : 0;

    return nHits;
}

//------------------------------------------------------------------------------------------------------------------------------------------

StatusCode MomentumThresholdAlgorithm::ReadSettings(const TiXmlHandle xmlHandle)
{
    PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, XmlHelper::ReadValue(xmlHandle, "CaloHitListName", m_caloHitListName));
    PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, XmlHelper::ReadValue(xmlHandle, "MCParticleListName", m_mcParticleListName));
    
    PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, XmlHelper::ReadValue(xmlHandle, "OutputTree", m_treeName));
    PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, XmlHelper::ReadValue(xmlHandle, "OutputFile", m_fileName));

    return STATUS_CODE_SUCCESS;
}

} // namespace analysis_content
