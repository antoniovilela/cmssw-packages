import FWCore.ParameterSet.Config as cms

edmDumpAnalysis = cms.EDAnalyzer("ExclusiveDijetsEdmDumpAnalyzer",
    # Input tags
    JetTag = cms.InputTag("L2L3CorJetSC7PF"),
    JetNonCorrTag = cms.InputTag("sisCone7PFJets"),
    ParticleFlowTag = cms.InputTag("particleFlow"),
    # Selection cuts
    PtMinJet = cms.double(50.0),
    EtaMaxJet = cms.double(2.5),
    DeltaEtaMax = cms.double(2.0),
    DeltaPhiMax = cms.double(0.4),
    DeltaPtMax = cms.double(999.),
    NTracksMax = cms.uint32(3),
    NHFPlusMax = cms.uint32(0),
    NHFMinusMax = cms.uint32(0),
    # B-tagging
    DoBTag = cms.bool(False),
    BTagMode = cms.string("2Btag"),
    BDiscriminatorName = cms.string("jetBProbabilityBJetTags"),
    BDiscMin = cms.double(3.0), 
    # Misc
    HFThresholdIndex = cms.uint32(16),
    UseJetCorrection = cms.bool(False),
    JetCorrectionService = cms.string("L2L3JetCorrectorSC7PF"),
    EBeam = cms.untracked.double(5000.),
    UsePAT = cms.untracked.bool(False)
)
