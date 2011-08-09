import FWCore.ParameterSet.Config as cms

from minimumBiasAnalysisTTree_cfg import process,config,processIdFilters
process.source.fileNames = ['file:/storage2/antoniov/data1/Pythia8MBR-reco423patch3/step2_0.root',
                            'file:/storage2/antoniov/data1/Pythia8MBR-reco423patch3/step2_1.root',
                            'file:/storage2/antoniov/data1/Pythia8MBR-reco423patch3/step2_10.root']
process.MessageLogger.cerr.threshold = 'INFO'
process.maxEvents.input = 3000
#process.GlobalTag.globaltag = 'START::All'
process.TFileService.fileName = "analysisMinBias_TTree_MinBias.root"
process.source.duplicateCheckMode = cms.untracked.string('noDuplicateCheck') 

"""
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

processIdFiltersPythia6 = ['processIdPythia6_SD','processIdPythia6_DD',
                            'processIdPythia6_Diff','processIdPythia6_ND']
processIdFiltersPythia8 = ['processIdPythia8_SD','processIdPythia8_DD',
                            'processIdPythia8_Diff','processIdPythia8_ND']
processIdFiltersPhojet = ['processIdPhojet_SD','processIdPhojet_DD',
                            'processIdPhojet_Diff','processIdPhojet_ND']
#processIdFilters = processIdFiltersPythia6
processIdFilters = processIdFiltersPythia8
#processIdFilters = processIdFiltersPhojet
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
"""
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

from Utilities.PyConfigTools.removeFromPaths import removeFromPaths
removeFromPaths(process,'bptx')
removeFromPaths(process,'hltBscMinBiasORBptxPlusORMinusFilter')

from Utilities.PyConfigTools.setAnalyzerAttributes import setAnalyzerAttributes
setAnalyzerAttributes(process,'minimumBiasTTreeAnalysis',
                               AccessMCInfo = True,
                               HLTPath = 'HLT_L1Tech_BSC_minBias_OR')
