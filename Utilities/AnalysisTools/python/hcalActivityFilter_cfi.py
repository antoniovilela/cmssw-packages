import FWCore.ParameterSet.Config as cms

hcalActivityFilter = cms.EDProducer("HCALActivityFilter",
    HCALTowerSummaryTag = cms.InputTag("hcalActivitySummary"),
    EnergyThresholdHB = cms.double(2.0),
    EnergyThresholdHE = cms.double(2.0),
    EnergyThresholdHF = cms.double(4.0),
    NTowersMaxHBPlus = cms.int32(-1),
    NTowersMaxHBMinus = cms.int32(-1),
    NTowersMaxHEPlus = cms.int32(-1),
    NTowersMaxHEMinus = cms.int32(-1),
    NTowersMaxHFPlus = cms.int32(-1),
    NTowersMaxHFMinus = cms.int32(-1),
    SumEMaxHBPlus = cms.double(-1.),
    SumEMaxHBMinus = cms.double(-1.),
    SumEMaxHEPlus = cms.double(-1.),
    SumEMaxHEMinus = cms.double(-1.),
    SumEMaxHFPlus = cms.double(-1.),
    SumEMaxHFMinus = cms.double(-1.)
)
