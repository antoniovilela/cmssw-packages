import FWCore.ParameterSet.Config as cms

process = cms.Process("Analysis")

process.load('FWCore.MessageService.MessageLogger_cfi')

process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10000) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        #'/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/RecoTracks-skim_GR09_P_V7_v1/0099/8EDF37E1-28E3-DE11-9C5D-001A92971B90.root'
        '/store/data/BeamCommissioning09/MinimumBias/RECO/v2/000/124/030/E6A8AB2F-78E7-DE11-973F-001617C3B710.root'
    )
)
# Run 124030
process.source.fileNames = (
    '/store/data/BeamCommissioning09/MinimumBias/RECO/Dec14thReReco_v1/0100/F6362E85-4BE9-DE11-B655-001D0967D558.root',
    '/store/data/BeamCommissioning09/MinimumBias/RECO/Dec14thReReco_v1/0100/90BC43B0-3BE9-DE11-AA67-001D0967CFB8.root',
    '/store/data/BeamCommissioning09/MinimumBias/RECO/Dec14thReReco_v1/0100/3E2566F2-38E9-DE11-AC46-001D0967DE45.root',
    '/store/data/BeamCommissioning09/MinimumBias/RECO/Dec14thReReco_v1/0100/2A5443BB-CCE9-DE11-B6C6-001D0967512F.root',
    '/store/data/BeamCommissioning09/MinimumBias/RECO/Dec14thReReco_v1/0099/E86399B9-32E9-DE11-8766-001D0967D2DD.root',
    '/store/data/BeamCommissioning09/MinimumBias/RECO/Dec14thReReco_v1/0099/9EC285D8-35E9-DE11-9B62-0024E87699A6.root',
    '/store/data/BeamCommissioning09/MinimumBias/RECO/Dec14thReReco_v1/0099/4CDA4BBE-32E9-DE11-8AD0-0024E8768099.root',
    '/store/data/BeamCommissioning09/MinimumBias/RECO/Dec14thReReco_v1/0099/3C40560F-36E9-DE11-9416-0024E876A855.root'
)

process.load("Configuration/StandardSequences/Geometry_cff")
process.load("Configuration/StandardSequences/MagneticField_cff")
process.load("Configuration/StandardSequences/FrontierConditions_GlobalTag_cff")
#process.GlobalTag.globaltag = 'GR09_P_V8::All'
process.GlobalTag.globaltag = 'GR09_R_V4::All'

process.load("MinimumBiasAnalysis.MinimumBiasAnalysis.analysisSequences_cff")
process.load("RecoMET.Configuration.RecoMET_BeamHaloId_cff")

process.load("RecoLocalTracker.SiPixelRecHits.PixelCPEESProducers_cff")
process.load("RecoLocalTracker.SiPixelRecHits.SiPixelRecHits_cfi")

process.pixelClusterSelection = cms.EDFilter("PixelClusterSelection",
    pixelRecHitsTag = cms.InputTag("siPixelRecHits"),
    #vertexTag = cms.InputTag("pixelVertices"),
    vertexTag = cms.InputTag("offlinePrimaryVertices"),
    applyfilter = cms.untracked.bool(False)
)

process.load("MinimumBiasAnalysis.MinimumBiasAnalysis.outputModule_cfi")
from MinimumBiasAnalysis.MinimumBiasAnalysis.minimumBiasEventContent_cff import MinimumBiasEventContent
process.output.outputCommands = MinimumBiasEventContent.outputCommands
process.output.fileName = '/tmp/antoniov/minimumBias_Run124030.root'
process.output.SelectEvents.SelectEvents = cms.vstring('selection_step')
#process.output.SelectEvents.SelectEvents = cms.vstring('*')
#process.output.SelectEvents.SelectEvents = cms.vstring('generalTracksPath','pixelTracksPath','pixelLessTracksPath')

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("analysisMinBias_histos.root")
)

##########
process.generalTracksFilter = cms.EDFilter("TrackCountFilter",
                                                      src = cms.InputTag('generalTracks'),
                                                      minNumber = cms.uint32(2) 
                                                      )
process.pixelLessTracksFilter = cms.EDFilter("TrackCountFilter",
                                                      src = cms.InputTag('ctfPixelLess'),
                                                      minNumber = cms.uint32(1) 
                                                      )
process.pixelTracksFilter = cms.EDFilter("TrackCountFilter",
                                                      src = cms.InputTag('pixelTracks'),
                                                      minNumber = cms.uint32(1) 
                                                      )

#process.generalTracksPath = cms.Path(process.generalTracksFilter)
#process.pixelTracksPath = cms.Path(process.pixelLessTracksFilter)
#process.pixelLessTracksPath = cms.Path(process.pixelTracksFilter)
#########

process.recoSequence = cms.Sequence(process.tracks*process.edmDump)

process.prereco_step = cms.Path(process.siPixelRecHits)
process.eventSelection_new = cms.Sequence(process.eventSelection+process.pixelClusterSelection)
process.selection_step = cms.Path(process.eventSelection_new)
process.reco_step = cms.Path(process.eventSelection_new+process.recoSequence+process.BeamHaloId)
#process.reco_step = cms.Path(process.recoSequence+process.BeamHaloId)

process.out_step = cms.EndPath(process.output)
#process.schedule = cms.Schedule(process.hlt_step,process.reco_step,process.analysis_step,process.out_step)
