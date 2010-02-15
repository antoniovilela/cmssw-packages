import FWCore.ParameterSet.Config as cms

from minimumBiasAnalysisTTree_cfg import process

process.maxEvents.input = 10000
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring()
)
from fileNames_MinBias_STARTUP3X_V8P_900GeV_Jan29ReReco import fileNames
process.source.fileNames = fileNames
process.GlobalTag.globaltag = 'STARTUP3X_V8P::All'
"""
AnalysisModules = ['minimumBiasTTreeAnalysis_eventSelection',
                   'minimumBiasTTreeAnalysis_eventSelectionMinBiasBSCOR',
                   'minimumBiasTTreeAnalysis_eventSelectionMinBiasPixel',
                   'minimumBiasTTreeAnalysis_eventSelectionMinBiasBSCORNoBPTX',
                   'minimumBiasTTreeAnalysis_eventSelectionMinBiasPixelNoBPTX']
"""
AnalysisModules = ['minimumBiasTTreeAnalysis_eventSelectionMinBiasBSCOR',
                   'minimumBiasTTreeAnalysis_eventSelectionMinBiasPixel']
for analysis in AnalysisModules:
    module = getattr(process,analysis)
    module.AccessMCInfo = True
    #module.TriggerResultsTag = cms.InputTag("TriggerResults::HLT8E29")
for pathname in process.paths:
    getattr(process,pathname).remove(getattr(process,'bptx'))
    getattr(process,pathname).remove(getattr(process,'lumiSectionSelection'))
    getattr(process,pathname).remove(getattr(process,'hltPhysicsDeclared'))

process.TFileService.fileName = "analysisMinBias_TTree_MinBias.root"
