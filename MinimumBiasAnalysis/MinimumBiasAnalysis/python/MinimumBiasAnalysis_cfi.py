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
    EnergyThresholdHBHE = cms.double(3.0),
    EnergyThresholdHF = cms.double(4.0),
    # Options
    EBeam = cms.double(3500.),
    ApplyEnergyScaleHCAL = cms.bool(False),
    EnergyScaleFactorHCAL = cms.double(1.0),
    HLTPath = cms.string("HLT_L1_BscMinBiasOR_BptxPlusORMinus"), 
    AccessMCInfo = cms.untracked.bool(False)
)
