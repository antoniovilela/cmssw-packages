import FWCore.ParameterSet.Config as cms

# Skim sequences
from minimumBiasAnalysis_cfg import process
if hasattr(process,'output'): del process.output
if hasattr(process,'out_step'): del process.out_step
process.maxEvents.input = 10000
process.xiTower.comEnergy = 900.0
process.xiFromCaloTowers.comEnergy = 900.0
process.xiFromJets.comEnergy = 900.0

from fileNames_MinimumBias_Jan29ReReco_124030 import fileNames
process.source.fileNames = fileNames
process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('124030:2-124030:9999','124027:24-124027:9999','124025:5-124025:13','124024:2-124024:83','124023:38-124023:9999','124022:66-124022:179','124020:12-124020:94')

process.load('MinimumBiasAnalysis.MinimumBiasAnalysis.minimumBiasTTreeAnalysis_cfi')

from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.analysisTools import *
#makeAnalysis(process,'minimumBiasTTreeAnalysis','eventSelection')
makeAnalysis(process,'minimumBiasTTreeAnalysis','eventSelectionMinBiasBSCOR')
makeAnalysis(process,'minimumBiasTTreeAnalysis','eventSelectionMinBiasPixel')

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("analysisMinBias_TTree_MinimumBias.root")
)
