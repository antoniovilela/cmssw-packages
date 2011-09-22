import FWCore.ParameterSet.Config as cms

pfThresholds = cms.PSet(
    Barrel = cms.PSet(
        hadronCharged = cms.PSet( pt = cms.double(0.0) ),
        hadronNeutral = cms.PSet( energy = cms.double(1.3) ),
        electron = cms.PSet( pt = cms.double(0.0) ),
        gamma = cms.PSet( energy = cms.double(0.4) )
    ),
    Endcap = cms.PSet(
        hadronCharged = cms.PSet( pt = cms.double(0.0) ),
        hadronNeutral = cms.PSet( energy = cms.double(2.6) ),
        electron = cms.PSet( pt = cms.double(0.0) ),
        gamma = cms.PSet( energy = cms.double(1.6) )
    ),
    Transition = cms.PSet(
        hadronCharged = cms.PSet( pt = cms.double(0.0) ),
        hadronNeutral = cms.PSet( energy = cms.double(2.6) ),
        electron = cms.PSet( pt = cms.double(0.0) ),
        gamma = cms.PSet( energy = cms.double(2.9) ),
        hadronHF = cms.PSet( energy = cms.double(3.0) ),
        emHF = cms.PSet( energy = cms.double(3.0) )
    ),
    Forward = cms.PSet(
        hadronHF = cms.PSet( energy = cms.double(3.5) ),
        emHF = cms.PSet( energy = cms.double(2.5) )
    )
)
