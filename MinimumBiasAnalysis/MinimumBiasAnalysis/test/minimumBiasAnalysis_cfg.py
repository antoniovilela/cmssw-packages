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
    '/store/data/BeamCommissioning09/MinimumBias/RECO/Dec19thReReco_336p3_v2/0103/D6FC56AE-ADEE-DE11-B2F7-0024E87680E7.root',
    '/store/data/BeamCommissioning09/MinimumBias/RECO/Dec19thReReco_336p3_v2/0102/E89FEAFF-23ED-DE11-9F78-0024E8768265.root',
    '/store/data/BeamCommissioning09/MinimumBias/RECO/Dec19thReReco_336p3_v2/0102/D6DE3CFE-19ED-DE11-97C6-001D0967D909.root',
    '/store/data/BeamCommissioning09/MinimumBias/RECO/Dec19thReReco_336p3_v2/0102/AE41D0D4-1FED-DE11-AF8C-0024E8767D04.root',
    '/store/data/BeamCommissioning09/MinimumBias/RECO/Dec19thReReco_336p3_v2/0102/8AECBAC5-1CED-DE11-8348-00151796D910.root',
    '/store/data/BeamCommissioning09/MinimumBias/RECO/Dec19thReReco_336p3_v2/0102/72F3F9B9-18ED-DE11-99A4-0015178C6B78.root',
    '/store/data/BeamCommissioning09/MinimumBias/RECO/Dec19thReReco_336p3_v2/0102/50B2D009-23ED-DE11-AE11-0024E8767D45.root',
    '/store/data/BeamCommissioning09/MinimumBias/RECO/Dec19thReReco_336p3_v2/0102/22299004-1BED-DE11-B488-0024E8767D04.root',
    '/store/data/BeamCommissioning09/MinimumBias/RECO/Dec19thReReco_336p3_v2/0102/1E35A2C5-1CED-DE11-BB47-0024E86E8D18.root',
    '/store/data/BeamCommissioning09/MinimumBias/RECO/Dec19thReReco_336p3_v2/0102/0A8CFAC4-18ED-DE11-8CD8-001D0967C1DF.root'
)

process.load("Configuration/StandardSequences/Geometry_cff")
process.load("Configuration/StandardSequences/MagneticField_cff")
process.load("Configuration/StandardSequences/FrontierConditions_GlobalTag_cff")
#process.GlobalTag.globaltag = 'GR09_P_V8::All'
process.GlobalTag.globaltag = 'GR09_R_V5::All'

process.load("MinimumBiasAnalysis.MinimumBiasAnalysis.analysisSequences_cff")
process.xiTower.comEnergy = 2360.0
process.xiFromCaloTowers.comEnergy = 2360.0
process.xiFromJets.comEnergy = 2360.0

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
process.output.fileName = 'minimumBias.root'
process.output.SelectEvents.SelectEvents = cms.vstring('selection_step')
#process.output.SelectEvents.SelectEvents = cms.vstring('*')
#process.output.SelectEvents.SelectEvents = cms.vstring('generalTracksPath','pixelTracksPath','pixelLessTracksPath')

#process.TFileService = cms.Service("TFileService",
#                                   fileName = cms.string("analysisMinBias_histos.root")
#)

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

#process.prereco_step = cms.Path(process.siPixelRecHits)
#process.eventSelection_new = cms.Sequence(process.eventSelection+process.pixelClusterSelection)
#process.selection_step = cms.Path(process.eventSelection_new)
#process.reco_step = cms.Path(process.eventSelection_new+process.recoSequence+process.BeamHaloId)

process.selection_step = cms.Path(process.eventSelection)
process.reco_step = cms.Path(process.recoSequence+process.BeamHaloId)

process.out_step = cms.EndPath(process.output)
#process.schedule = cms.Schedule(process.hlt_step,process.reco_step,process.analysis_step,process.out_step)
