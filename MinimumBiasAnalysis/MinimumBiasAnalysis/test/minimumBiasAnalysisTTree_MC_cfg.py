import FWCore.ParameterSet.Config as cms

from minimumBiasAnalysisTTree_cfg import process
process.source.fileNames = ['file:/tmp/antoniov/MinBias_TuneD6T_7TeV-pythia6_START3X_V26B-v1_F4FB0378-445F-DF11-84A1-003048779609.root']
process.MessageLogger.cerr.threshold = 'INFO'
process.maxEvents.input = -1
process.GlobalTag.globaltag = 'START3X_V26B::All'

analyzerPrefix = 'minimumBiasTTreeAnalysis'
for analyzerName in process.analyzers:
    if analyzerName.find(analyzerPrefix) != -1:
        module = getattr(process,analyzerName)
        module.AccessMCInfo = True
        #module.TriggerResultsTag = cms.InputTag("TriggerResults::SIM")
for pathName in process.paths:
    getattr(process,pathName).remove(getattr(process,'bptx'))

process.TFileService.fileName = "analysisMinBias_TTree_MinBias.root"
