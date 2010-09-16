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

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.GeometryExtended_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.Reconstruction_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
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

###################################################################################
# HCAL reflagging
# HF RecHit reflagger -- specify type of HF cleaning to use
from Utilities.AnalysisTools.addHcalReflagging import addHFReflagging,addHBHEReflagging
process = addHFReflagging(process,version=10,isMC=False)
# HBHE RecHit reflagger
process = addHBHEReflagging(process)

print "These are the severity levels for the various rechit flags:"
print "(Severity > 10 causes rechit to be ignored by CaloTower maker)"
for i in process.hcalRecAlgos.SeverityLevels: print i

# Use the reflagged HF RecHits to make the CaloTowers
process.towerMaker.hfInput = "hfrecoReflagged"
process.towerMakerWithHO.hfInput = "hfrecoReflagged"
# Use the reflagged HBHE RecHits to make the CaloTowers
process.towerMaker.hbheInput = "hbherecoReflagged"
process.towerMakerWithHO.hbheInput = "hbherecoReflagged"

###################################################################################

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

# Reflagging and re-reco
process.reflagging_step = cms.Path(process.hfrecoReflagged+process.hbherecoReflagged)
process.rereco_step = cms.Path(process.caloTowersRec
                               *(process.recoJets*process.recoJetIds+process.recoTrackJets)
                               *process.recoJetAssociations
                               *process.metreco
                               ) # re-reco jets and met

process.selection_step = cms.Path(process.eventSelectionBscMinBiasOR)
process.reco_step = cms.Path(process.eventSelection+process.recoSequence)

process.out_step = cms.EndPath(process.output)
#process.schedule = cms.Schedule(process.selection_step,process.reco_step,process.analysis_step,process.out_step)
