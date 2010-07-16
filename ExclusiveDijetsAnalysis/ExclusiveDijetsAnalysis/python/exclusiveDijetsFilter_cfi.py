import FWCore.ParameterSet.Config as cms

exclusiveDijetsFilter = cms.EDAnalyzer("ExclusiveDijetsFilter",
    # Input tags
    JetTag = cms.InputTag("selectedLayer1JetsSC7PF"),
    TrackMultiplicityTag = cms.InputTag("trackMultiplicityTransverseRegion","trackMultiplicity"),
    # Selection cuts
    PtMinJet = cms.double(40.0),
    EtaMaxJet = cms.double(3.0),
    DeltaEtaMax = cms.double(999.),
    DeltaPhiMax = cms.double(999.),
    DeltaPtMax = cms.double(999.),
    ThirdJetPtMax = cms.double(9999.),
    NTracksMax = cms.uint32(9999),
    NHFPlusMax = cms.uint32(9999),
    NHFMinusMax = cms.uint32(9999),
    # B-tagging
    DoBTag = cms.bool(False),
    BTagMode = cms.string("2Btag"),
    BDiscriminatorName = cms.string("jetBProbabilityBJetTags"),
    BDiscMin = cms.double(3.0), 
    # Misc
    HFThresholdIndex = cms.uint32(15),
    UsePAT = cms.untracked.bool(True)
)
