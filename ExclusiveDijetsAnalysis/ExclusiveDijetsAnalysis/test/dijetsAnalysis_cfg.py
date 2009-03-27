import FWCore.ParameterSet.Config as cms

process = cms.Process("Analysis")

process.load('FWCore.MessageService.MessageLogger_cfi')
process.MessageLogger.debugModules = cms.untracked.vstring('')
process.MessageLogger.cerr.threshold = 'INFO'

process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('')
)

process.load("ExclusiveDijetsAnalysis.ExclusiveDijetsAnalysis.exclusiveDijetsHLTPaths_cfi")

process.load("JetMETCorrections.Configuration.L2L3Corrections_Summer08_cff")
process.prefer("L2L3JetCorrectorIC5PF")

process.analysis = cms.EDAnalyzer("SimpleDijetsAnalyzer",
    JetTag = cms.InputTag("L2L3JetCorrectorIC5PF")
)

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("analysis_histos.root")
)

process.p = cms.Path(process.exclusiveDijetsHLTFilter*process.L2L3JetCorrectorIC5PF*process.analysis)
