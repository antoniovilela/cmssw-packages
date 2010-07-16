import FWCore.ParameterSet.Config as cms

ExclusiveDijetsAnalysis = cms.PSet(
    # Input tags
    JetTag = cms.InputTag("selectedLayer1JetsSC7PF"),
    JetNonCorrTag = cms.InputTag("sisCone7PFJets"),
    ParticleFlowTag = cms.InputTag("particleFlow"),
    VertexTag = cms.InputTag("offlinePrimaryVertices"),
    TrackMultiplicityTag = cms.InputTag("trackMultiplicityTransverseRegion","trackMultiplicity"),
    # B-tagging
    DoBTag = cms.bool(True),
    BDiscriminatorName = cms.string("jetBProbabilityBJetTags"),
    # Misc
    HFThresholdIndex = cms.uint32(15),
    UseJetCorrection = cms.bool(False),
    JetCorrectionService = cms.string("L2L3JetCorrectorSC7PF"),
    # If accessing the pile-up info
    AccessPileUpInfo = cms.bool(False),
    EBeam = cms.untracked.double(5000.),
    UsePAT = cms.untracked.bool(True),
    RunOnData = cms.untracked.bool(False)
)
