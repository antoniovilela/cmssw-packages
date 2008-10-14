import FWCore.ParameterSet.Config as cms

process = cms.Process("Analysis")
# keep the logging output to a nice level ###
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.debugModules = cms.untracked.vstring('pileupanalysis')
#process.MessageLogger.cerr.threshold = 'DEBUG'
process.MessageLogger.cerr.threshold = 'INFO'
#process.MessageLogger.cerr.threshold = 'WARNING'
process.MessageLogger.categories.append('Analysis')
process.MessageLogger.cerr.DEBUG = cms.untracked.PSet(
    default = cms.untracked.PSet( limit = cms.untracked.int32(0)),
    Analysis = cms.untracked.PSet( limit = cms.untracked.int32(-1))
)
process.MessageLogger.cerr.INFO = cms.untracked.PSet(
    default = cms.untracked.PSet( limit = cms.untracked.int32(0)),
    Analysis = cms.untracked.PSet( limit = cms.untracked.int32(1000000))
)

process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

# Tracking Truth and mixing module, if needed
#process.load("SimGeneral.MixingModule.mixNoPU_cfi")

#process.load("SimGeneral.TrackingAnalysis.trackingParticles_cfi")

process.load("Configuration.StandardSequences.MagneticField_cff") 
process.load("SimTracker.TrackAssociation.TrackAssociatorByHits_cfi")
#process.load("SimTracker.TrackAssociation.TrackAssociatorByChi2_cfi")
process.load("SimTracker.VertexAssociation.VertexAssociatorByTracks_cfi")
process.load("SimTracker.TrackAssociation.trackingParticleRecoTrackAsssociation_cfi")
#process.TrackAssociatorByHits.

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('file:/tmp/antoniov/PYTHIA6_EWK_Wmunu_10TeV_InitialLumPU_cff_py_RAW2DIGI_RECO_FEVTDEBUGEventContent.root')
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)
process.pileupanalysis = cms.EDFilter("PileUpAnalysis",
                              TracksTag = cms.InputTag("generalTracks"),
                              VerticesTag = cms.InputTag("offlinePrimaryVertices"),
			      TrackAssociatorTag = cms.InputTag("trackingParticleRecoTrackAsssociation"),
                              BunchCrossings = cms.vint32(-1,0,1)
)

process.add_(cms.Service("TFileService",
		fileName = cms.string("analysisPileUp_histos.root")
	)
)

#process.p = cms.EndPath(process.mix*process.trackingParticles*process.pileupanalysis)

#process.p = cms.Path(process.trackingParticles*process.pileupanalysis)

process.p = cms.Path(process.trackingParticleRecoTrackAsssociation*process.pileupanalysis)

#process.p = cms.Path(process.pileupanalysis)
