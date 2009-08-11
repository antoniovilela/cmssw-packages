import FWCore.ParameterSet.Config as cms

exclusiveDijetsFilter = cms.EDAnalyzer("ExclusiveDijetsFilter",
    # Input tags
    JetTag = cms.InputTag("selectedLayer1JetsSC7PF"),
    # Selection cuts
    PtMinJet = cms.double(50.0),
    EtaMaxJet = cms.double(2.5),
    DeltaEtaMax = cms.double(2.0),
    DeltaPhiMax = cms.double(0.4),
    DeltaPtMax = cms.double(999.),
    ThirdJetPtMax = cms.double(9999.),
    NTracksMax = cms.uint32(7),
    NHFPlusMax = cms.uint32(10),
    NHFMinusMax = cms.uint32(10),
    # B-tagging
    DoBTag = cms.bool(False),
    BTagMode = cms.string("2Btag"),
    BDiscriminatorName = cms.string("jetBProbabilityBJetTags"),
    BDiscMin = cms.double(3.0), 
    # Misc
    HFThresholdIndex = cms.uint32(16),
    UsePAT = cms.untracked.bool(True)
)
