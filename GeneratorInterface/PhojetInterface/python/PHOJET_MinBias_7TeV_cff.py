import FWCore.ParameterSet.Config as cms

source = cms.Source("PhojetSource",
    pythiaPylistVerbosity = cms.untracked.int32(2),
    pythiaHepMCVerbosity = cms.untracked.bool(True),
    maxEventsToPrint = cms.untracked.int32(2),
    comEnergy = cms.untracked.double(7000.0),
    PhojetParameters = cms.PSet(
        parameterSets = cms.vstring("minBiasParameters"),
        minBiasParameters = cms.vstring(
            'PROCESS     1  0  1  1  1  1  1  1  1  1  1'
        )
    )
)
