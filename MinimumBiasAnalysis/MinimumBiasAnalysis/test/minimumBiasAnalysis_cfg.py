import FWCore.ParameterSet.Config as cms

process = cms.Process("Analysis")

process.load('FWCore.MessageService.MessageLogger_cfi')
process.MessageLogger.cerr.threshold = 'INFO'

process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(50000) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        #'/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/RecoTracks-skim_GR09_P_V7_v1/0099/8EDF37E1-28E3-DE11-9C5D-001A92971B90.root'
        '/store/data/BeamCommissioning09/MinimumBias/RECO/v2/000/124/030/E6A8AB2F-78E7-DE11-973F-001617C3B710.root'
    )
)
#from fileNames_ZeroBias_124023_Lumi_41_96 import fileNames as filesZeroBias 
#from fileNames_MinimumBias_124023_Lumi_41_96 import fileNames as filesMinimumBias
from fileNames_MinimumBias_Jan29ReReco_124023_Lumi_41_96 import fileNames as filesMinimumBias
#process.source.fileNames = filesZeroBias
process.source.fileNames = filesMinimumBias

process.load("Configuration/StandardSequences/Geometry_cff")
process.load("Configuration/StandardSequences/MagneticField_cff")
process.load("Configuration/StandardSequences/FrontierConditions_GlobalTag_cff")
#process.GlobalTag.globaltag = 'GR09_P_V8::All'
process.GlobalTag.globaltag = 'GR09_R_V6A::All'

process.load("MinimumBiasAnalysis.MinimumBiasAnalysis.analysisSequences_cff")
#process.xiTower.comEnergy = 2360.0
#process.xiFromCaloTowers.comEnergy = 2360.0
#process.xiFromJets.comEnergy = 2360.0

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
#process.output.fileName = 'minimumBias.root'
process.output.fileName = 'minimumBias_MinimumBias_Jan29ReReco_124023_Lumi_41_96.root'
#process.output.fileName = '/tmp/antoniov/minimumBias.root'
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

"""
hltPaths = ('hltMinBiasBSCOR',
            'hltMinBiasPixel',
            'hltMinBiasBSCORNoColl',
            'hltMinBiasPixelNoColl',
            'hltMinBiasBSCORNoBPTX',
            'hltMinBiasPixelNoBPTX')
from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.analysisTools import addPath
for path in hltPaths:
    addPath(process,getattr(process,path))
"""
#process.HLT_MinBiasBSCOR = cms.Path(process.bscOr)
#process.HLT_MinBiasPixel = cms.Path(process.hltMinBiasPixelSingleTrackFilter)

#process.selection_step = cms.Path(process.eventSelection)
process.selection_step = cms.Path(process.eventSelectionMinBiasBSCOR)
process.reco_step = cms.Path(process.eventSelection+process.recoSequence+process.BeamHaloId)

#process.selection_step.replace(process.eventSelection,process.eventSelectionNoColl)
#process.reco_step.replace(process.eventSelection,process.eventSelectionNoColl)
#process.selection_step.replace(process.eventSelection,process.eventSelectionNoBPTX)
#process.reco_step.replace(process.eventSelection,process.eventSelectionNoBPTX)
#process.selection_step.replace(process.eventSelection,process.hltNoColl)
#process.reco_step.replace(process.eventSelection,process.hltNoColl)

process.out_step = cms.EndPath(process.output)
#process.schedule = cms.Schedule(process.hlt_step,process.reco_step,process.analysis_step,process.out_step)
