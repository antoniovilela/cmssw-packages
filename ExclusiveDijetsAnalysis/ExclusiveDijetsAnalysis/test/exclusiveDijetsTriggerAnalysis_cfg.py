import FWCore.ParameterSet.Config as cms

# Settings
class config: pass
config.verbose = True
config.globalTagName = 'GR_R_42_V19::All' 
config.inputFileName = '/storage2/eliza/JetMay10RecoRun2011.root'
config.outputTTreeFile = 'exclusiveDijetsTriggerAnalysis.root'

process = cms.Process("Analysis")

process.load('FWCore.MessageService.MessageLogger_cfi')
process.MessageLogger.cerr.threshold = 'DEBUG' 
process.MessageLogger.debugModules = cms.untracked.vstring('dijetsTriggerAnalysisHFRingEtSums')
#process.MessageLogger.destinations = cms.untracked.vstring('cerr')
process.MessageLogger.categories.append('Analysis')
process.MessageLogger.cerr.DEBUG = cms.untracked.PSet(limit = cms.untracked.int32(0))
process.MessageLogger.cerr.Analysis = cms.untracked.PSet(limit = cms.untracked.int32(-1))
#process.MessageLogger.debugs = cms.untracked.PSet(
#    threshold = cms.untracked.string('DEBUG'),
#    noLineBreaks = cms.untracked.bool(False),
#    DEBUG = cms.untracked.PSet(limit = cms.untracked.int32(0)),
#    INFO = cms.untracked.PSet(limit = cms.untracked.int32(0)),
#    Analysis = cms.untracked.PSet(limit = cms.untracked.int32(-1))
#)

process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True),
                                      SkipEvent = cms.untracked.vstring('ProductNotFound') )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring( 'file:%s' % config.inputFileName )
)

#-------------------------------------------------------------------------------
# Import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.GeometryExtended_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.Reconstruction_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('JetMETCorrections.Configuration.DefaultJEC_cff')
#process.load("CondCore.DBCommon.CondDBCommon_cfi")

#--------------------------------------------------------------------------------
process.GlobalTag.globaltag = config.globalTagName
#---------------------------------------------------------------------------------
# Output
process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string(config.outputTTreeFile))

###################################################################################
# Analysis modules
#--------------------------------
process.load('ExclusiveDijetsAnalysis.ExclusiveDijetsAnalysis.dijetsTriggerAnalysis_cfi')
process.load('ForwardAnalysis.ForwardTTreeAnalysis.exclusiveDijetsAnalysisSequences_cff')
process.exclusiveDijetsHLTFilter.HLTPaths = ['HLT_Jet60_v*']

# HFRingEtSums,HFBitCounts
process.dijetsTriggerAnalysisHFRingEtSums = process.dijetsTriggerAnalysis.clone( hfRingSumType = "HFRingEtSums",
                                                                                 hfRingThreshold = 2)

process.dijetsTriggerAnalysisHFBitCounts = process.dijetsTriggerAnalysis.clone( hfRingSumType = "HFBitCounts",
                                                                                hfRingThreshold = 5)

process.analysis_step = cms.Path(process.exclusiveDijetsHLTFilter+
                                 process.dijetsTriggerAnalysisHFRingEtSums+
                                 process.dijetsTriggerAnalysisHFBitCounts)
