import FWCore.ParameterSet.Config as cms

edmDumpAnalysis = cms.EDAnalyzer("ExclusiveDijetsEdmDumpAnalyzer",
    JetTag = cms.InputTag("L2L3CorJetSC7PF"),
    ParticleFlowTag = cms.InputTag("particleFlow"),
    PtMinJet = cms.double(50.0),
    EtaMaxJet = cms.double(2.5),
    DeltaEtaMax = cms.double(2.0),
    DeltaPhiMax = cms.double(0.4),
    DeltaPtMax = cms.double(999.),
    NTracksMax = cms.uint32(3),
    NHFPlusMax = cms.uint32(1),
    NHFMinusMax = cms.uint32(1),
    HFThresholdIndex = cms.uint32(12),
    UseJetCorrection = cms.bool(False),
    #JetCorrectionService = cms.string("L2L3JetCorrectorSC5PF"),
    EBeam = cms.untracked.double(5000.),
    UsePAT = cms.untracked.bool(False)
)
