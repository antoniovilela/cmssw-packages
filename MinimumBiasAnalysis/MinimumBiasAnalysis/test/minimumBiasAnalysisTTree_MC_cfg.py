import FWCore.ParameterSet.Config as cms

from minimumBiasAnalysisTTree_cfg import process
process.MessageLogger.cerr.threshold = 'INFO'
process.maxEvents.input = 10000
process.GlobalTag.globaltag = 'START3X_V26B::All'

analyzerPrefix = 'minimumBiasTTreeAnalysis'
for analyzerName in process.analyzers:
    if analyzerName.find(analyzerPrefix) != -1:
        module = getattr(process,analyzerName)
        module.AccessMCInfo = True
        #module.TriggerResultsTag = cms.InputTag("TriggerResults::")
for pathName in process.paths:
    getattr(process,pathName).remove(getattr(process,'bptx'))

process.TFileService.fileName = "analysisMinBias_TTree_MinBias.root"
