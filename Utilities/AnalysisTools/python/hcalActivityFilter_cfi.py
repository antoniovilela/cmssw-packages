import FWCore.ParameterSet.Config as cms

hcalActivityFilter = cms.EDProducer("HCALActivityFilter",
    HCALTowerSummaryTag = cms.InputTag("hcalActivitySummary"),
    EnergyThresholdHB = cms.double(1.25),
    EnergyThresholdHE = cms.double(1.9),
    EnergyThresholdHF = cms.double(4.0),
    EnergyThresholdEB = cms.double(0.6),
    EnergyThresholdEE = cms.double(2.45), 
    NTowersMaxHBPlus = cms.int32(-1),
    NTowersMaxHBMinus = cms.int32(-1),
    NTowersMaxHEPlus = cms.int32(-1),
    NTowersMaxHEMinus = cms.int32(-1),
    NTowersMaxHFPlus = cms.int32(-1),
    NTowersMaxHFMinus = cms.int32(-1),
    NTowersMaxEBPlus = cms.int32(-1),
    NTowersMaxEBMinus = cms.int32(-1),
    NTowersMaxEEPlus = cms.int32(-1),
    NTowersMaxEEMinus = cms.int32(-1),
    SumEMaxHBPlus = cms.double(-1.),
    SumEMaxHBMinus = cms.double(-1.),
    SumEMaxHEPlus = cms.double(-1.),
    SumEMaxHEMinus = cms.double(-1.),
    SumEMaxHFPlus = cms.double(-1.),
    SumEMaxHFMinus = cms.double(-1.),
    SumEMaxEBPlus = cms.double(-1.),
    SumEMaxEBMinus = cms.double(-1.),
    SumEMaxEEPlus = cms.double(-1.),
    SumEMaxEEMinus = cms.double(-1.),
    SumETMaxHBPlus = cms.double(-1.),
    SumETMaxHBMinus = cms.double(-1.),
    SumETMaxHEPlus = cms.double(-1.),
    SumETMaxHEMinus = cms.double(-1.),
    SumETMaxHFPlus = cms.double(-1.),
    SumETMaxHFMinus = cms.double(-1.),
    SumETMaxEBPlus = cms.double(-1.),
    SumETMaxEBMinus = cms.double(-1.),
    SumETMaxEEPlus = cms.double(-1.),
    SumETMaxEEMinus = cms.double(-1.)
)
