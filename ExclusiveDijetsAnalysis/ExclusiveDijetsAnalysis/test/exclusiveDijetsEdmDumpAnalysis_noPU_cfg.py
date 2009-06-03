import FWCore.ParameterSet.Config as cms

process = cms.Process("EdmDumpAnalysis")

process.load('FWCore.MessageService.MessageLogger_cfi')
process.MessageLogger.debugModules = cms.untracked.vstring('analysis')
process.MessageLogger.cerr.threshold = 'INFO'
process.MessageLogger.categories.append('Analysis')
process.MessageLogger.cerr.DEBUG = cms.untracked.PSet(
    default = cms.untracked.PSet( limit = cms.untracked.int32(0)),
    Analysis = cms.untracked.PSet( limit = cms.untracked.int32(-1))
)

process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('')
)

process.load("JetMETCorrections.Configuration.L2L3Corrections_Summer08Redigi_cff")

process.load("ExclusiveDijetsAnalysis.ExclusiveDijetsAnalysis.edmDumpAnalysis_cfi")

process.load("DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.singleVertexFilter_cfi")

attributes = [{'NHFPlusMax':0,'NHFMinusMax':0},
              {'NHFPlusMax':1,'NHFMinusMax':1},
              {'NHFPlusMax':2,'NHFMinusMax':2},
              {'NHFPlusMax':3,'NHFMinusMax':3},
              {'NHFPlusMax':4,'NHFMinusMax':4},
              {'NHFPlusMax':5,'NHFMinusMax':5},
              {'HFThresholdIndex':10},
              {'HFThresholdIndex':12},
              {'HFThresholdIndex':14},
              {'HFThresholdIndex':16}]

from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.analysisTools import *

makeAnalysis(process,'edmDumpAnalysis',attributes,[],'singleVertexFilter')

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("analysisDijets_histos_PU.root")
)
