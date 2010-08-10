import FWCore.ParameterSet.Config as cms

from minimumBiasAnalysisTTree_cfg import process,config
#process.source.fileNames = ['file:/tmp/antoniov/MinBias_TuneD6T_7TeV-pythia6_START36_V10_SP10-v1_GEN-SIM-RECODEBUG_F63DF090-6879-DF11-9E7D-0030487CDA68.root']
#process.source.fileNames = ['file:/tmp/antoniov/MinBias_TuneCW_7TeV-pythia6_START36_V10-v1_GEN-SIM-RECODEBUG_F65EF1B2-4887-DF11-9A50-001EC9AAA38C.root']
process.source.fileNames = ['file:/tmp/antoniov/MinBias_7TeV-pythia8_START36_V10_SP10-v1_GEN-SIM-RECODEBUG_F2E56105-8E74-DF11-9C73-00237DA1CDBE.root']
#process.source.fileNames = ['file:/tmp/antoniov/PHOJET_MinBias_7TeV_START36_V10_362-HLT-v1_PrivateProd-START36_V10_362_RECO-v1_step2_RAW2DIGI_L1Reco_RECO_VALIDATION_5_1_5XQ.root']
process.MessageLogger.cerr.threshold = 'INFO'
process.maxEvents.input = -1
process.GlobalTag.globaltag = 'START36_V10::All'

analyzerPrefix = 'minimumBiasTTreeAnalysis'
for analyzerName in process.analyzers:
    if analyzerName.find(analyzerPrefix) != -1:
        module = getattr(process,analyzerName)
        module.AccessMCInfo = True
        #module.HLTPath = 'HLT_MinBiasBSC_OR'
        module.TriggerResultsTag = cms.InputTag("TriggerResults::REDIGI36")
for pathName in process.paths:
    getattr(process,pathName).remove(getattr(process,'bptx'))
    getattr(process,pathName).remove(getattr(process,'hltBscMinBiasORBptxPlusORMinusFilter'))

# Add paths for different process-Id selections
from Utilities.AnalysisTools.processIdFilter_cfi import processIdFilter
# Pythia 6
process.processIdPythia6_SD = processIdFilter.clone(ProcessIds = [92,93])
process.processIdPythia6_DD = processIdFilter.clone(ProcessIds = [94])
process.processIdPythia6_Diff = processIdFilter.clone(ProcessIds = [92,93,94])
process.processIdPythia6_ND = cms.Sequence(~process.processIdPythia6_Diff)
# Pythia 8
process.processIdPythia8_SD = processIdFilter.clone(ProcessIds = [103,104])
process.processIdPythia8_DD = processIdFilter.clone(ProcessIds = [105])
process.processIdPythia8_Diff = processIdFilter.clone(ProcessIds = [103,104,105])
process.processIdPythia8_ND = cms.Sequence(~process.processIdPythia8_Diff)
# Phojet
process.processIdPhojet_SD = processIdFilter.clone(GeneratorTag = "source", ProcessIds = [5,6])
process.processIdPhojet_DD = processIdFilter.clone(GeneratorTag = "source", ProcessIds = [7])
process.processIdPhojet_Diff = processIdFilter.clone(GeneratorTag = "source", ProcessIds = [4,5,6,7])
process.processIdPhojet_ND = cms.Sequence(~process.processIdPhojet_Diff)

filtersProcessIdsPythia6 = ['processIdPythia6_SD','processIdPythia6_DD',
                            'processIdPythia6_Diff','processIdPythia6_ND']
filtersProcessIdsPythia8 = ['processIdPythia8_SD','processIdPythia8_DD',
                            'processIdPythia8_Diff','processIdPythia8_ND']
filtersProcessIdsPhojet = ['processIdPhojet_SD','processIdPhojet_DD',
                            'processIdPhojet_Diff','processIdPhojet_ND']
#filtersProcessIds = filtersProcessIdsPythia6
filtersProcessIds = filtersProcessIdsPythia8
#filtersProcessIds = filtersProcessIdsPhojet

from Utilities.PyConfigTools.analysisTools import *
makeAnalysis(process,'minimumBiasTTreeAnalysis','eventSelectionBscMinBiasOR',filters=filtersProcessIds)
makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasOR',filters=filtersProcessIds)
makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasORHFVetoPlus',filters=filtersProcessIds)
makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasORHFVetoMinus',filters=filtersProcessIds)
makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasORHEHFVetoPlus',filters=filtersProcessIds)
makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasORHEHFVetoMinus',filters=filtersProcessIds)
makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasORSumEMaxHFPlus4',filters=filtersProcessIds)
makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasORSumEMaxHFPlus8',filters=filtersProcessIds)
makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasORSumEMaxHFPlus12',filters=filtersProcessIds)
makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasORSumEMaxHFPlus16',filters=filtersProcessIds)
makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasORSumEMaxHFMinus4',filters=filtersProcessIds)
makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasORSumEMaxHFMinus8',filters=filtersProcessIds)
makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasORSumEMaxHFMinus12',filters=filtersProcessIds)
makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasORSumEMaxHFMinus16',filters=filtersProcessIds)

process.TFileService.fileName = "analysisMinBias_TTree_MinBias.root"
