import FWCore.ParameterSet.Config as cms

process = cms.Process("TEST")
# keep the logging output to a nice level ###
process.load("FWCore.MessageService.MessageLogger_cfi")

# Tracking Truth and mixing module, if needed
#process.load("SimGeneral.MixingModule.mixNoPU_cfi")

process.load("SimGeneral.TrackingAnalysis.trackingParticles_cfi")

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('file:/tmp/antoniov/PYTHIA6_EWK_Wmunu_10TeV_InitialLumPU_cff_GEN_SIM_DIGI_L1_DIGI2RAW_HLT.root')
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)
process.myTest = cms.EDFilter("PileUpAnalysis")

#process.p = cms.EndPath(process.mix*process.trackingParticles*process.myTest)

process.p = cms.Path(process.trackingParticles*process.myTest)
