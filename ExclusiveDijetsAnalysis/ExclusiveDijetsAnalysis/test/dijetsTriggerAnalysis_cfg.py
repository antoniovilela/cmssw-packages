import FWCore.ParameterSet.Config as cms

process = cms.Process("Analysis")

process.load('FWCore.MessageService.MessageLogger_cfi')
process.MessageLogger.debugModules = cms.untracked.vstring('analysis')
process.MessageLogger.cerr.threshold = 'DEBUG'
#process.MessageLogger.cerr.INFO.limit = -1

process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('file:/tmp/antoniov/QCDpt30_Summer08_IDEAL_V11_redigi_v1_RECO_345194AC-4EE3-DD11-9EF6-001D0967D634.root')
)

#process.load("ExclusiveDijetsAnalysis.ExclusiveDijetsAnalysis.exclusiveDijetsHLTPaths_cfi")

process.load("DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.pileUpInfo_cfi")
from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.pileUpNumberFilter_cfi import *
process.filter0PU = pileUpNumberFilter.clone(NumberOfPileUpEvents = 0)

process.analysis = cms.EDAnalyzer("DijetsTriggerAnalyzer",
  #bit counts per ring
  HFRingETSumThreshold = cms.int32(1),
  AccessL1GctHFRingEtSums = cms.untracked.bool(False)
)

process.analysis0PU = process.analysis.clone()

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("analysisDijetsTrigger_histos.root")
)

process.analysis0PU_seq = cms.Sequence(process.pileUpInfo*process.filter0PU*process.analysis0PU)
process.analysis_step = cms.Path(process.analysis+process.analysis0PU_seq)
