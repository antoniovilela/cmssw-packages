import FWCore.ParameterSet.Config as cms

from minimumBiasAnalysisTTree_cfg import process,config
process.source.fileNames = ['file:/storage2/antoniov/data1/Pythia8MBR-reco423patch3/step2_0.root',
                            'file:/storage2/antoniov/data1/Pythia8MBR-reco423patch3/step2_1.root',
                            'file:/storage2/antoniov/data1/Pythia8MBR-reco423patch3/step2_10.root']
process.MessageLogger.cerr.threshold = 'INFO'
process.maxEvents.input = 3000
#process.GlobalTag.globaltag = 'START::All'
process.TFileService.fileName = "analysisMinBias_TTree_MinBias.root"
process.source.duplicateCheckMode = cms.untracked.string('noDuplicateCheck') 

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
makeAnalysis(process,'minimumBiasTTreeAnalysis','eventSelectionBscMinBiasOREtaMaxFilter',filters=filtersProcessIds)
makeAnalysis(process,'minimumBiasTTreeAnalysis','eventSelectionBscMinBiasOREtaMinFilter',filters=filtersProcessIds)
makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasOR',filters=filtersProcessIds)
makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasOREtaMaxFilter',filters=filtersProcessIds)
makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasOREtaMinFilter',filters=filtersProcessIds)
"""
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
"""

analyzerPrefix = 'minimumBiasTTreeAnalysis'
for analyzerName in process.analyzers:
    if analyzerName.find(analyzerPrefix) != -1:
        module = getattr(process,analyzerName)
        module.AccessMCInfo = True
        module.HLTPath = 'HLT_Jet30_v*'
        #module.TriggerResultsTag = cms.InputTag("TriggerResults::REDIGI36")
for pathName in process.paths:
    getattr(process,pathName).remove(getattr(process,'bptx'))
    getattr(process,pathName).remove(getattr(process,'hltBscMinBiasORBptxPlusORMinusFilter'))
