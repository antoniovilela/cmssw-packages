import FWCore.ParameterSet.Config as cms
import math

process = cms.Process("Analysis")

process.load('FWCore.MessageService.MessageLogger_cfi')
process.MessageLogger.debugModules = cms.untracked.vstring('triggerAnalyzer*')
process.MessageLogger.destinations = cms.untracked.vstring('cerr')
process.MessageLogger.categories.append('Analysis')
process.MessageLogger.cerr =  cms.untracked.PSet(
    threshold = cms.untracked.string('DEBUG'),
    noLineBreaks = cms.untracked.bool(False),
    DEBUG = cms.untracked.PSet(limit = cms.untracked.int32(0)),
    INFO = cms.untracked.PSet(limit = cms.untracked.int32(0)),
    FwkJob = cms.untracked.PSet( reportEvery = cms.untracked.int32(1000) ),
    FwkReport = cms.untracked.PSet( reportEvery = cms.untracked.int32(1000) ),
    Analysis = cms.untracked.PSet(limit = cms.untracked.int32(-1))
)

process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.GlobalTag.connect   = 'frontier://FrontierProd/CMS_COND_31X_GLOBALTAG'
process.GlobalTag.pfnPrefix = cms.untracked.string('frontier://FrontierProd/')
from Configuration.AlCa.autoCond import autoCond
process.GlobalTag.globaltag = autoCond['hltonline'].split(',')[0]

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("analysisTrigger_histos.root")
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('file:')
)

process.triggerAnalyzer = cms.EDAnalyzer('SimpleTriggerAnalyzer',
    #gtDigisTag = cms.InputTag('simGtDigis'),
    gtDigisTag = cms.InputTag('hltGtDigis'),
    l1ExtraParticlesTag = cms.InputTag('hltL1extraParticles'),
    hltJetTag = cms.InputTag('hltCaloJetL1FastJetCorrected'),
    l1TriggerNames = cms.vstring(),
    l1SingleJetEtMin = cms.double(0.),
    l1DoubleJetEtMin = cms.double(0.),
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

process.triggerAnalyzerL1SingleJet16 = process.triggerAnalyzer.clone( l1TriggerNames = ['L1_SingleJet16'] ) 
process.triggerAnalyzerL1SingleJet36 = process.triggerAnalyzer.clone( l1TriggerNames = ['L1_SingleJet36'] ) 
process.triggerAnalyzerL1DoubleJet20 = process.triggerAnalyzer.clone( l1TriggerNames = ['L1_DoubleJet20'] ) 
process.triggerAnalyzerL1DoubleJet24 = process.triggerAnalyzer.clone( l1TriggerNames = ['L1_DoubleJet24'] ) 
#process.triggerAnalyzerL1DoubleJet36Central = process.triggerAnalyzer.clone( l1TriggerNames = ['L1_DoubleJet36_Central'] ) 

import copy
from HLTrigger.HLTfilters.hltHighLevel_cfi import *
hltFilter = copy.deepcopy(hltHighLevel)
process.hltZeroBias = hltFilter.clone( HLTPaths = ['HLT_ZeroBias_v*'] )

process.analysis_step = cms.Path( #process.hltZeroBias +
                                  process.triggerAnalyzerL1SingleJet16 + 
                                  process.triggerAnalyzerL1SingleJet36 +
                                  process.triggerAnalyzerL1DoubleJet20 +
                                  process.triggerAnalyzerL1DoubleJet24 )
