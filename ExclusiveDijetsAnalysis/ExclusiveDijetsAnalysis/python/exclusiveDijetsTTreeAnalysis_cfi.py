import FWCore.ParameterSet.Config as cms

exclusiveDijetsTTreeAnalysis = cms.EDAnalyzer("ExclusiveDijetsTTreeAnalyzer",
    # Input tags
    JetTag = cms.InputTag("selectedLayer1JetsSC7PF"),
    JetNonCorrTag = cms.InputTag("sisCone7PFJets"),
    ParticleFlowTag = cms.InputTag("particleFlow"),
    VertexTag = cms.InputTag("offlinePrimaryVertices"),
    # B-tagging
    DoBTag = cms.bool(True),
    BDiscriminatorName = cms.string("jetBProbabilityBJetTags"),
    # Misc
    HFThresholdIndex = cms.uint32(16),
    UseJetCorrection = cms.bool(False),
    JetCorrectionService = cms.string("L2L3JetCorrectorSC7PF"),
    # If accessing the pile-up info
    AccessPileUpInfo = cms.bool(True),
    EBeam = cms.untracked.double(5000.),
    UsePAT = cms.untracked.bool(True),
    # Saves or not ROOT TTree
    SaveROOTTree = cms.untracked.bool(True)
)