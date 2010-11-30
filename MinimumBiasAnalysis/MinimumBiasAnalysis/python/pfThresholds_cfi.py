import FWCore.ParameterSet.Config as cms

pfThresholds = cms.PSet(
    hadronCharged = cms.PSet(
        ptMin = cms.double(0.5),
        energyMin = cms.double(0.0) 
    ),
    hadronNeutral = cms.PSet(
        ptMin = cms.double(0.0),
        energyMin = cms.double(2.0)
    ),
    electron = cms.PSet(
        ptMin = cms.double(0.5),
        energyMin = cms.double(0.0)
    ),
    gamma = cms.PSet(
        ptMin = cms.double(0.0),
        energyMin = cms.double(2.0) 
    ),
    hadronHF = cms.PSet(
        ptMin = cms.double(0.0),
        energyMin = cms.double(4.0)
    ),
    emHF = cms.PSet(
        ptMin = cms.double(0.0),
        energyMin = cms.double(4.0)
    )
)
