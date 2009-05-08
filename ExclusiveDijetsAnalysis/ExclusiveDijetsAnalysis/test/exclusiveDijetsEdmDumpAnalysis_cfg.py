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
    fileNames = cms.untracked.vstring(
       "rfio:/castor/cern.ch/user/a/antoniov/crab_output/crab_ExclusiveDijetsStageA156BxPU_M100_HLTDijetAve30_v3/edmDump_exclusiveDijets_1.root",
       "rfio:/castor/cern.ch/user/a/antoniov/crab_output/crab_ExclusiveDijetsStageA156BxPU_M100_HLTDijetAve30_v3/edmDump_exclusiveDijets_2.root",
       "rfio:/castor/cern.ch/user/a/antoniov/crab_output/crab_ExclusiveDijetsStageA156BxPU_M100_HLTDijetAve30_v3/edmDump_exclusiveDijets_3.root",
       "rfio:/castor/cern.ch/user/a/antoniov/crab_output/crab_ExclusiveDijetsStageA156BxPU_M100_HLTDijetAve30_v3/edmDump_exclusiveDijets_4.root",
       "rfio:/castor/cern.ch/user/a/antoniov/crab_output/crab_ExclusiveDijetsStageA156BxPU_M100_HLTDijetAve30_v3/edmDump_exclusiveDijets_5.root"
    )
)

process.load("JetMETCorrections.Configuration.L2L3Corrections_Summer08Redigi_cff")

from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.pileUpNumberFilter_cfi import *
process.filter0PU = pileUpNumberFilter.clone(NumberOfPileUpEvents = 0)

process.analysis = cms.EDAnalyzer("ExclusiveDijetsEdmDumpAnalyzer",
    JetTag = cms.InputTag("selectedLayer1Jets"),
    ParticleFlowTag = cms.InputTag("particleFlow"),
    PtMinJet = cms.double(50.0),
    EtaMaxJet = cms.double(2.5),
    DeltaEtaMax = cms.double(1.4),
    DeltaPhiMax = cms.double(0.2),
    NTracksMax = cms.uint32(3),
    NHFPlusMax = cms.uint32(0),
    NHFMinusMax = cms.uint32(0),
    HFThresholdIndex = cms.uint32(12),
    UseJetCorrection = cms.bool(False),
    #JetCorrectionService = cms.string("L2L3JetCorrectorSC5PF")
)

process.analysis0PU = process.analysis.clone()

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("analysisDijets_histos_PU.root")
)

process.analysis_AvePU = cms.Path(process.analysis)
process.analysis_0PU = cms.Path(process.filter0PU+process.analysis0PU)
