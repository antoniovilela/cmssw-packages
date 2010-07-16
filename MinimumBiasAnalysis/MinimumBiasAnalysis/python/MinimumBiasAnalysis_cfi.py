import FWCore.ParameterSet.Config as cms

MinimumBiasAnalysis = cms.PSet(
    # Input tags
    VertexTag = cms.InputTag("offlinePrimaryVertices"),
    JetTag = cms.InputTag("ak5CaloJets"),
    METTag = cms.InputTag("met"),
    CaloTowerTag = cms.InputTag("towerMaker"), 
    ParticleFlowTag = cms.InputTag("particleFlow"),
    # Misc
    HFThresholdIndex = cms.uint32(15),
    EnergyThresholdHBHE = cms.double(-1.),
    EnergyThresholdHF = cms.double(3.0),
    # Options
    EBeam = cms.double(450.),
    AccessMCInfo = cms.untracked.bool(False)
)
