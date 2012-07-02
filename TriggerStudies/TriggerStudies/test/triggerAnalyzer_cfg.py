import FWCore.ParameterSet.Config as cms
import math

process = cms.Process("Analysis")

process.load('FWCore.MessageService.MessageLogger_cfi')
#process.MessageLogger.debugModules = cms.untracked.vstring('analysis')
#process.MessageLogger.cerr.threshold = 'INFO'
#process.MessageLogger.cerr.INFO.limit = -1

process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("analysisTrigger_histos.root")
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('file:')
)

process.triggerAnalyzer = cms.EDAnalyzer('SimpleTriggerAnalyzer',
    hltJetTag = cms.InputTag(''),
    minPt = cms.double(0.),
    maxPt = cms.double(100.),
    nBinsPt = cms.uint32(100),
    minEta = cms.double(-5.0),
    maxEta = cms.double(5.0),
    nBinsEta = cms.uint32(100),
    minPhi = cms.double(-math.pi),
    maxPhi = cms.double(math.pi),
    nBinsPhi = cms.uint32(100)
)

process.analysis_step = cms.Path( process.triggerAnalyzer )
