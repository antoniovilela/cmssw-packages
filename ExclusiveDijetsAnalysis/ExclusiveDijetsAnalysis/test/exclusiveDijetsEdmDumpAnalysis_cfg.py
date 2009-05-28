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

from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.pileUpNumberFilter_cfi import *
filtersPU = []
for i in range(5):
    filtersPU.append('filter%dPU'%i)
    setattr(process,'filter%dPU'%i,pileUpNumberFilter.clone(NumberOfPileUpEvents = i))

process.edmDumpAnalysis = cms.EDAnalyzer("ExclusiveDijetsEdmDumpAnalyzer",
    JetTag = cms.InputTag("L2L3CorJetSC7PF"),
    ParticleFlowTag = cms.InputTag("particleFlow"),
    PtMinJet = cms.double(50.0),
    EtaMaxJet = cms.double(2.5),
    DeltaEtaMax = cms.double(2.0),
    DeltaPhiMax = cms.double(0.4),
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

filters = filtersPU[:]

makeAnalysis(process,'edmDumpAnalysis',attributes,filters)

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("analysisDijets_histos_PU.root")
)

"""
process.analysis_AvePU = cms.Path(process.analysis)
process.analysis_0PU = cms.Path(process.filter0PU+process.analysis0PU)
"""
