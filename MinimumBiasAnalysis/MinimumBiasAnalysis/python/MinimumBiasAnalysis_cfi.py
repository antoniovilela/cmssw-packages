import FWCore.ParameterSet.Config as cms

MinimumBiasAnalysis = cms.PSet(
    # Input tags
    VertexTag = cms.InputTag("offlinePrimaryVertices"),
    TrackTag = cms.InputTag("analysisTracks"),
    JetTag = cms.InputTag("ak5PFJets"),
    METTag = cms.InputTag("met"),
    CaloTowerTag = cms.InputTag("towerMaker"), 
    ParticleFlowTag = cms.InputTag("particleFlow"),
    TriggerResultsTag = cms.InputTag("TriggerResults::HLT"),
    HCALTowerSummaryTag = cms.InputTag("hcalActivitySummary"),
    # Misc
    EnergyThresholdHB = cms.double(1.5),
    EnergyThresholdHE = cms.double(2.0),
    EnergyThresholdHF = cms.double(4.0),
    # Options
    EBeam = cms.double(3500.),
    ApplyEnergyScaleHCAL = cms.bool(False),
    EnergyScaleFactorHCAL = cms.double(1.0),
    HLTPath = cms.string("HLT_L1_BscMinBiasOR_BptxPlusORMinus"), 
    AccessMCInfo = cms.untracked.bool(False)
)
