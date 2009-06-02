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

from ExclusiveDijetsAnalysis.ExclusiveDijetsAnalysis.filesCEPGGM100_edmDump_cfi import filesPSet

process.source = cms.Source("PoolSource",
    fileNames = filesPSet.fileNames
)

process.load("JetMETCorrections.Configuration.L2L3Corrections_Summer08Redigi_cff")

process.load("ExclusiveDijetsAnalysis.ExclusiveDijetsAnalysis.edmDumpAnalysis_cfi")

from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.pileUpNumberFilter_cfi import *
filtersPU = []
for i in range(5):
    filtersPU.append('filter%dPU'%i)
    setattr(process,'filter%dPU'%i,pileUpNumberFilter.clone(NumberOfPileUpEvents = i))

attributes = [{'NHFPlusMax':0,'NHFMinusMax':0},
              {'NHFPlusMax':1,'NHFMinusMax':1},
              {'NHFPlusMax':2,'NHFMinusMax':2}]

from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.analysisTools import *

filters = filtersPU[:]

makeAnalysis(process,'edmDumpAnalysis',attributes,filters)

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("analysisDijets_histos_PU.root")
)
