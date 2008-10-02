import FWCore.ParameterSet.Config as cms

process = cms.Process("Analysis")
# keep the logging output to a nice level ###
process.load("FWCore.MessageService.MessageLogger_cfi")

# Tracking Truth and mixing module, if needed
#process.load("SimGeneral.MixingModule.mixNoPU_cfi")

#process.load("SimGeneral.TrackingAnalysis.trackingParticles_cfi")

process.load("Configuration.StandardSequences.MagneticField_cff") 
process.load("SimTracker.TrackAssociation.TrackAssociatorByHits_cfi")
#process.load("SimTracker.TrackAssociation.trackingParticleRecoTrackAsssociation_cfi")

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('file:/tmp/antoniov/PYTHIA6_EWK_Wmunu_10TeV_InitialLumPU_cff_py_RAW2DIGI_RECO.root')
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)
process.pileupanalysis = cms.EDFilter("PileUpAnalysis",
                              TracksTag = cms.InputTag("generalTracks"),
			      TrackAssociatorTag = cms.InputTag("trackingParticleRecoTrackAsssociation")	 
)

#process.p = cms.EndPath(process.mix*process.trackingParticles*process.pileupanalysis)

#process.p = cms.Path(process.trackingParticles*process.pileupanalysis)

#process.p = cms.Path(process.trackingParticleRecoTrackAsssociation*process.pileupanalysis)

process.p = cms.Path(process.pileupanalysis)
