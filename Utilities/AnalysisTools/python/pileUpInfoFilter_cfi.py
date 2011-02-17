import FWCore.ParameterSet.Config as cms

pileUpInfoFilter = cms.EDFilter("PileUpInfoFilter",
    PileUpInfoTag = cms.InputTag("addPileupInfo"),
    numberOfPileUpEvents = cms.uint32(0)
)
