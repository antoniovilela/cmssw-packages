import FWCore.ParameterSet.Config as cms

MinimumBiasAnalysis = cms.PSet(
    # Input tags
    VertexTag = cms.InputTag("offlinePrimaryVertices"),
    JetTag = cms.InputTag("ak5CaloJets"),
    METTag = cms.InputTag("met"),
    CaloTowerTag = cms.InputTag("towerMaker"), 
    ParticleFlowTag = cms.InputTag("particleFlow"),
    TriggerResultsTag = cms.InputTag("TriggerResults::HLT"),
    HFTowerSummaryTag = cms.InputTag("hfTower"),
    # Misc
    HFThresholdIndex = cms.uint32(20),
    EnergyThresholdHBHE = cms.double(1.5),
    EnergyThresholdHF = cms.double(4.0),
    # Options
    EBeam = cms.double(450.),
    ApplyEnergyScaleHCAL = cms.bool(False),
    EnergyScaleFactorHCAL = cms.double(1.0),
    AccessMCInfo = cms.untracked.bool(False)
)
