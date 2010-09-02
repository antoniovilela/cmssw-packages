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

process.load("Configuration/StandardSequences/Geometry_cff")
process.load("Configuration/StandardSequences/MagneticField_cff")
process.load("Configuration/StandardSequences/FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = ''

process.load("MinimumBiasAnalysis.MinimumBiasAnalysis.analysisSequences_cff")
#process.xiTower.comEnergy = 2360.0
#process.xiFromCaloTowers.comEnergy = 2360.0
#process.xiFromJets.comEnergy = 2360.0

#process.load("RecoMET.Configuration.RecoMET_BeamHaloId_cff")

process.load("MinimumBiasAnalysis.MinimumBiasAnalysis.outputModule_cfi")
from MinimumBiasAnalysis.MinimumBiasAnalysis.minimumBiasEventContent_cff import MinimumBiasEventContent
process.output.outputCommands = MinimumBiasEventContent.outputCommands
process.output.fileName = 'minimumBias.root'
process.output.SelectEvents.SelectEvents = cms.vstring('selection_step')

#process.TFileService = cms.Service("TFileService",
#                                   fileName = cms.string("analysisMinBias_histos.root")
#)

# HCAL reflagging

process.recoSequence = cms.Sequence(process.tracks*process.edmDump)

"""
hltPaths = ('hltMinBiasBSCOR',
            'hltMinBiasPixel',
            'hltMinBiasBSCORNoColl',
            'hltMinBiasPixelNoColl',
            'hltMinBiasBSCORNoBPTX',
            'hltMinBiasPixelNoBPTX')
from Utilities.PyConfigTools.analysisTools import addPath
for path in hltPaths:
    addPath(process,getattr(process,path))
"""

process.selection_step = cms.Path(process.eventSelectionBscMinBiasOR)
process.reco_step = cms.Path(process.eventSelection+process.recoSequence)

process.out_step = cms.EndPath(process.output)
#process.schedule = cms.Schedule(process.selection_step,process.reco_step,process.analysis_step,process.out_step)
