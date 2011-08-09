import FWCore.ParameterSet.Config as cms

from MinimumBiasAnalysis.MinimumBiasAnalysis.pfThresholds_cfi import pfThresholds
#from ForwardAnalysis.Utilities.pfThresholds_cfi import pfThresholds

MinimumBiasAnalysis = cms.PSet(
    # Input tags
    VertexTag = cms.InputTag("offlinePrimaryVertices"),
    TrackTag = cms.InputTag(""),
    JetTag = cms.InputTag("ak5PFJets"),
    METTag = cms.InputTag("met"),
    CaloTowerTag = cms.InputTag("towerMaker"), 
    CastorRecHitTag = cms.InputTag("castorRecHitCorrector"),
    ParticleFlowTag = cms.InputTag("particleFlow"),
    GenChargedParticlesTag = cms.InputTag("genChargedParticles"),
    TriggerResultsTag = cms.InputTag("TriggerResults","","HLT"),
    HCALTowerSummaryTag = cms.InputTag("hcalActivitySummary"),
    # Misc
    EnergyThresholdHB = cms.double(1.5),
    EnergyThresholdHE = cms.double(2.0),
    EnergyThresholdHF = cms.double(4.0),
    # PF thresholds
    PFlowThresholds = pfThresholds,
    # Options
    EBeam = cms.double(3500.),
    ApplyEnergyScaleHCAL = cms.bool(False),
    EnergyScaleFactorHCAL = cms.double(1.0),
    HLTPath = cms.string("HLT_L1_BscMinBiasOR_BptxPlusORMinus"), 
    AccessMCInfo = cms.untracked.bool(False)
)
