import FWCore.ParameterSet.Config as cms

process = cms.Process("Analysis")

process.load('FWCore.MessageService.MessageLogger_cfi')
process.MessageLogger.debugModules = cms.untracked.vstring('l1filter')
process.MessageLogger.cerr.threshold = 'DEBUG'
#process.MessageLogger.cerr.INFO.limit = -1

process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('file:/tmp/antoniov/QCDpt30_Summer08_IDEAL_V11_redigi_v1_RECO_345194AC-4EE3-DD11-9EF6-001D0967D634.root')
)

process.load("ExclusiveDijetsAnalysis.ExclusiveDijetsAnalysis.exclusiveDijetsHLTPaths_cfi")

process.analysis = cms.EDAnalyzer("DijetsTriggerAnalyzer",
  #bit counts per ring
  HFRingETSumThreshold = cms.int32(0),
  AccessL1GctHFRingEtSums = cms.untracked.bool(False)
)

process.analysisHLT = process.analysis.clone()

process.l1filter = cms.EDFilter("L1TriggerTestFilter",
  HFRingETSumThreshold = cms.int32(0),
  L1TriggerNames = cms.vstring("L1_SingleJet30"),
  AccessL1GctHFRingEtSums = cms.untracked.bool(False) 
)

process.l1filterHLT = process.l1filter.clone()

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("analysisDijetsTrigger_histos.root")
)

process.hlt = cms.Sequence(process.exclusiveDijetsHLTFilter)
process.analysis_step = cms.Path(process.analysis)
process.analysis_step_hlt = cms.Path(process.hlt + process.analysisHLT)

process.filter = cms.Path(process.l1filter)
process.filter_hlt = cms.Path(process.hlt + process.l1filterHLT)
