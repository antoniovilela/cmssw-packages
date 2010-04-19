import FWCore.ParameterSet.Config as cms

from minimumBiasAnalysisTTree_cfg import process

process.maxEvents.input = 10000
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring()
)
from fileNames_MinBias_STARTUP3X_V8P_900GeV_Jan29ReReco import fileNames
process.source.fileNames = fileNames
#process.GlobalTag.globaltag = 'STARTUP3X_V8P::All'
process.GlobalTag.globaltag = 'STARTUP3X_V8O::All'

"""
AnalysisModules = ['minimumBiasTTreeAnalysis_eventSelection',
                   'minimumBiasTTreeAnalysis_eventSelectionMinBiasBSCOR',
                   'minimumBiasTTreeAnalysis_eventSelectionMinBiasPixel']
"""

analyzerPrefix = 'minimumBiasTTreeAnalysis'
for analyzerName in process.analyzers:
    if analyzerName.find(analyzerPrefix) != -1:
        module = getattr(process,analyzerName)
        module.AccessMCInfo = True
        #module.TriggerResultsTag = cms.InputTag("TriggerResults::HLT8E29")
for pathName in process.paths:
    getattr(process,pathName).remove(getattr(process,'bptx'))
    getattr(process,pathName).remove(getattr(process,'lumiSectionSelection'))
    getattr(process,pathName).remove(getattr(process,'hltPhysicsDeclared'))

process.TFileService.fileName = "analysisMinBias_TTree_MinBias.root"
