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

from Utilities.AnalysisTools.processIdFilter_cfi import processIdFilter
process.processIdPythia6_SD = processIdFilter.clone(ProcessIds = [92,93])
process.processIdPythia6_DD = processIdFilter.clone(ProcessIds = [94])
process.processIdPythia6_Diff = processIdFilter.clone(ProcessIds = [92,93,94])
process.processIdPythia8_SD = processIdFilter.clone(ProcessIds = [103,104])
process.processIdPythia8_DD = processIdFilter.clone(ProcessIds = [105])
process.processIdPythia8_Diff = processIdFilter.clone(ProcessIds = [103,104,105])
filtersProcessIdsPythia6 = ['processIdPythia6_SD','processIdPythia6_DD','processIdPythia6_Diff']
filtersProcessIdsPythia8 = ['processIdPythia8_SD','processIdPythia8_DD','processIdPythia8_Diff']
filtersProcessIds = filtersProcessIdsPythia6

from Utilities.PyConfigTools.analysisTools import *
makeAnalysis(process,'minimumBiasTTreeAnalysis','eventSelectionBscMinBiasOR',filters=filtersProcessIds)
makeAnalysis(process,'trackHistos','eventSelectionBscMinBiasOR',filters=filtersProcessIds)
makeAnalysis(process,'trackHistos','eventSelectionBscMinBiasORHFVetoPlus',filters=filtersProcessIds)
makeAnalysis(process,'trackHistos','eventSelectionBscMinBiasORHFVetoMinus',filters=filtersProcessIds)
makeAnalysis(process,'trackHistos','eventSelectionBscMinBiasORHEHFVetoPlus',filters=filtersProcessIds)
makeAnalysis(process,'trackHistos','eventSelectionBscMinBiasORHEHFVetoMinus',filters=filtersProcessIds)

process.TFileService.fileName = "analysisMinBias_TTree_MinBias.root"
