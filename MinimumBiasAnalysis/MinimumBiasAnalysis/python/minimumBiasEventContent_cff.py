import FWCore.ParameterSet.Config as cms

MinimumBiasEventContent = cms.PSet(
    outputCommands = cms.untracked.vstring(
        'drop *',
        # Tracks custom
        'keep *_selectTracksAssociatedToPV_*_Analysis',
        # Jets
        'keep *_kt6CaloJets_*_*',
        'keep *_ak5CaloJets_*_*',
        'keep *_ak7CaloJets_*_*',
        'keep *_sisCone5CaloJets_*_*', 
        'keep *_sisCone7CaloJets_*_*',  
        'keep *_kt4PFJets_*_*', 
        'keep *_kt6PFJets_*_*',
        'keep *_ak5PFJets_*_*',
        'keep *_ak7PFJets_*_*',
        'keep *_sisCone5PFJets_*_*', 
        'keep *_sisCone7PFJets_*_*',
        # User variables
        'keep *_trackMultiplicity_*_Analysis',
        'keep *_trackMultiplicityAssociatedToPV_*_Analysis',
        'keep *_trackMultiplicityOutsideJets_*_Analysis',
        'keep *_trackMultiplicityTransverseRegion_*_Analysis',
        'keep *_hfTower_*_Analysis',
        'keep *_xiTower_*_Analysis',
        'keep *_xiFromCaloTowers_*_Analysis',
        'keep *_xiFromJets_*_Analysis',
        # RECO
        'keep *_EventAuxilary_*_*',
        'keep edmTriggerResults_TriggerResults_*_*',
        'keep *_hltTriggerSummaryAOD_*_*',
        'keep L1GlobalTriggerObjectMapRecord_*_*_*',
        'keep L1GlobalTriggerReadoutRecord_*_*_*',
        'keep *_hcalnoise_*_*',
        'keep *_CSCHaloData_*_*',
        'keep *_EcalHaloData_*_*',
        'keep *_HcalHaloData_*_*',
        'keep *_GlobalHaloData_*_*',
        'keep *_BeamHaloSummary_*_*',
        'keep recoTracks_generalTracks_*_*',
        'keep recoTracks_ctfPixelLess_*_*',
        'keep recoTracks_pixelTracks_*_*',
        'keep *_pixelVertices_*_*',
        'keep *_offlinePrimaryVertices_*_*',
        'keep *_particleFlow_*_*',
        'keep *_caloTowers_*_*', 
        'keep *_towerMaker_*_*' 
    )    
)

from GeneratorInterface.Configuration.GeneratorInterface_EventContent_cff import GeneratorInterfaceRECO
MinimumBiasEventContent.outputCommands.extend(GeneratorInterfaceRECO.outputCommands)

from RecoMET.Configuration.RecoMET_EventContent_cff import RecoMETRECO
MinimumBiasEventContent.outputCommands.extend(RecoMETRECO.outputCommands)
