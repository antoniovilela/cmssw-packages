import FWCore.ParameterSet.Config as cms

from minimumBiasAnalysisTTree_cfg import process,config,processIdFilters
from fileNames_Pythia8MBR_reco423patch3 import fileNames
#process.source.fileNames = ['file:/storage2/antoniov/data1/Pythia8MBR-reco423patch3/step2_0.root',
#                            'file:/storage2/antoniov/data1/Pythia8MBR-reco423patch3/step2_1.root',
#                            'file:/storage2/antoniov/data1/Pythia8MBR-reco423patch3/step2_10.root']
process.source.fileNames = fileNames
process.MessageLogger.cerr.threshold = 'INFO'
process.maxEvents.input = 3000
#process.GlobalTag.globaltag = 'START::All'
#process.source.duplicateCheckMode = cms.untracked.string('noDuplicateCheck') 
process.TFileService.fileName = "/storage2/antoniov/data1/AnalysisResults/Pythia8MBR_reco423patch3/minimumBiasTTreeAnalysis-v3/analysisMinBias_TTree_MinBias_Pythia8MBR-reco423patch3.root"

"""
from Utilities.PyConfigTools.analysisTools import *
makeAnalysis(process,'minimumBiasTTreeAnalysis','eventSelectionBscMinBiasOR',filters=processIdFilters)
makeAnalysis(process,'minimumBiasTTreeAnalysis','eventSelectionBscMinBiasOREtaMaxFilter',filters=processIdFilters)
makeAnalysis(process,'minimumBiasTTreeAnalysis','eventSelectionBscMinBiasOREtaMinFilter',filters=processIdFilters)
makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasOR',filters=processIdFilters)
makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasOREtaMaxFilter',filters=processIdFilters)
makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasOREtaMinFilter',filters=processIdFilters)
makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasOREtaMinFilterCastorVeto',filters=processIdFilters)
makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasOREtaMinFilterCastorTag',filters=processIdFilters)
makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasORHFVetoPlus',filters=processIdFilters)
makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasORHFVetoMinus',filters=processIdFilters)
makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasORHEHFVetoPlus',filters=processIdFilters)
makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasORHEHFVetoMinus',filters=processIdFilters)
makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasORSumEMaxHFPlus4',filters=processIdFilters)
makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasORSumEMaxHFPlus8',filters=processIdFilters)
makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasORSumEMaxHFPlus12',filters=processIdFilters)
makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasORSumEMaxHFPlus16',filters=processIdFilters)
makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasORSumEMaxHFMinus4',filters=processIdFilters)
makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasORSumEMaxHFMinus8',filters=processIdFilters)
makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasORSumEMaxHFMinus12',filters=processIdFilters)
makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasORSumEMaxHFMinus16',filters=processIdFilters)
"""

"""
from Utilities.PyConfigTools.removeFromPaths import removeFromPaths
removeFromPaths(process,'bptx')
removeFromPaths(process,'hltBscMinBiasORBptxPlusORMinusFilter')

from Utilities.PyConfigTools.setAnalyzerAttributes import setAnalyzerAttributes
setAnalyzerAttributes(process,'minimumBiasTTreeAnalysis',
                               AccessMCInfo = True,
                               HLTPath = 'HLT_L1Tech_BSC_minBias_OR')
"""
