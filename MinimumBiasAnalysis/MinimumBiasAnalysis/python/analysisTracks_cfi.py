import FWCore.ParameterSet.Config as cms

analysisTracks = cms.EDFilter("TrackSelector",
    src = cms.InputTag("generalTracks"),
    cut = cms.string('pt > 0. & numberOfValidHits > 7 & d0 <= 3.5 & dz <= 30')
)
