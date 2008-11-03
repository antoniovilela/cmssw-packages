import FWCore.ParameterSet.Config as cms

process = cms.Process("Analysis")
# keep the logging output to a nice level ###
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.debugModules = cms.untracked.vstring('pileupanalysis')
#process.MessageLogger.cerr.threshold = 'DEBUG'
#process.MessageLogger.cerr.threshold = 'INFO'
process.MessageLogger.cerr.threshold = 'WARNING'
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


process.load("Configuration.StandardSequences.MagneticField_cff") 
process.load("SimTracker.TrackAssociation.TrackAssociatorByHits_cfi")
#process.load("SimTracker.TrackAssociation.TrackAssociatorByChi2_cfi")
process.load("SimTracker.VertexAssociation.VertexAssociatorByTracks_cfi")
process.load("SimTracker.TrackAssociation.trackingParticleRecoTrackAsssociation_cfi")

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('file:/tmp/antoniov/PYTHIA6_EWK_Wmunu_10TeV_InitialLumPU_cff_py_RAW2DIGI_RECO_FEVTDEBUGEventContent.root')
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.pileUpAnalysis = cms.EDAnalyzer("SimplePileUpAnalyzer",
                              TracksTag = cms.InputTag("generalTracks"),
                              VerticesTag = cms.InputTag("offlinePrimaryVertices"),
                              TrackAssociatorTag = cms.InputTag("trackingParticleRecoTrackAsssociation"),
                              CaloTowersTag = cms.InputTag("towerMaker"),
                              GenParticlesTag = cms.InputTag("genParticles"),
                              BunchCrossings = cms.vint32(0)
                              #BunchCrossings = cms.vint32(-1,0,1)
)

process.vtxEffAnalysis = cms.EDAnalyzer("VertexEfficiencyAnalyzer",
                              TracksTag = cms.InputTag("generalTracks"),
                              VerticesTag = cms.InputTag("offlinePrimaryVertices"),
                              TrackAssociatorTag = cms.InputTag("trackingParticleRecoTrackAsssociation")
)

process.trkEffAnalysis = cms.EDAnalyzer("TrackEfficiencyAnalyzer",
                              TracksTag = cms.InputTag("generalTracks"),
                              VerticesTag = cms.InputTag("offlinePrimaryVertices"),
                              TrackAssociatorTag = cms.InputTag("trackingParticleRecoTrackAsssociation")
)

process.add_(cms.Service("TFileService",
		fileName = cms.string("analysisJetsPileUp_histos.root")
	)
)

#process.selection = cms.Sequence(process.)
process.p = cms.Path(process.trackingParticleRecoTrackAsssociation*process.vtxEffAnalysis*process.trkEffAnalysis*process.pileUpAnalysis)

