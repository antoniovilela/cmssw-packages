import FWCore.ParameterSet.Config as cms

process = cms.Process("Analysis")

process.load('FWCore.MessageService.MessageLogger_cfi')
process.MessageLogger.debugModules = cms.untracked.vstring('analysis')
process.MessageLogger.cerr.threshold = 'INFO'
process.MessageLogger.cerr.INFO.limit = -1

process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('file:/tmp/antoniov/QCD100to250-madgraph_IDEAL_V12_FastSim_GEN-SIM-DIGI-RECO_FCE6F1F2-FE1C-DE11-9095-0019B9CB01A7.root')
)

process.load("ExclusiveDijetsAnalysis.ExclusiveDijetsAnalysis.exclusiveDijetsHLTPaths_cfi")

process.analysis = cms.EDAnalyzer("DijetsTriggerAnalyzer",
  GTDigisTag = cms.InputTag("gtDigis"),
  L1GTObjectMapTag = cms.InputTag("gtDigis"), 
  #bit counts per ring
  HFRingETSumThreshold = cms.int32(0),
  AccessL1GctHFRingEtSums = cms.untracked.bool(True)
)

process.analysisHLT = process.analysis.clone()

process.l1filter = cms.EDFilter("L1TriggerTestFilter",
  HFRingETSumThreshold = cms.int32(0),
  L1TriggerNames = cms.vstring("L1_SingleJet30"),
  AccessL1GctHFRingEtSums = cms.untracked.bool(True) 
)

process.l1filterHLT = process.l1filter.clone()

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("analysisDijetsTrigger_histos.root")
)

process.hlt = cms.Sequence(process.exclusiveDijetsHLTFilter)
process.analysis_step = cms.Path(process.analysis)
process.analysis_step_hlt = cms.Path(process.hlt + process.analysisHLT)

#process.filter = cms.Path(process.l1filter)
#process.filter_hlt = cms.Path(process.hlt + process.l1filterHLT)
