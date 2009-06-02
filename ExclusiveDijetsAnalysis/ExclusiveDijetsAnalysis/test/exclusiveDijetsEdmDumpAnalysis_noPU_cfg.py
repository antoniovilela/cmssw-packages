import FWCore.ParameterSet.Config as cms

process = cms.Process("EdmDumpAnalysis")

process.load('FWCore.MessageService.MessageLogger_cfi')
process.MessageLogger.debugModules = cms.untracked.vstring('analysis')
process.MessageLogger.cerr.threshold = 'WARNING'
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

process.edmDumpAnalysis = cms.EDAnalyzer("ExclusiveDijetsEdmDumpAnalyzer",
    JetTag = cms.InputTag("L2L3CorJetSC7PF"),
    ParticleFlowTag = cms.InputTag("particleFlow"),
    PtMinJet = cms.double(50.0),
    EtaMaxJet = cms.double(2.5),
    DeltaEtaMax = cms.double(2.0),
    DeltaPhiMax = cms.double(0.4),
    DeltaPtMax = cms.double(999.),
    NTracksMax = cms.uint32(3),
    NHFPlusMax = cms.uint32(1),
    NHFMinusMax = cms.uint32(1),
    HFThresholdIndex = cms.uint32(12),
    UseJetCorrection = cms.bool(False),
    #JetCorrectionService = cms.string("L2L3JetCorrectorSC5PF"),
    EBeam = cms.untracked.double(5000.),
    UsePAT = cms.untracked.bool(False)
)

attributes = {}
from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.analysisTools import *

filters = []

makeAnalysis(process,'edmDumpAnalysis',attributes,filters)

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("analysisDijets_histos_noPU.root")
)
