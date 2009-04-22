import FWCore.ParameterSet.Config as cms

process = cms.Process("EdmDumpAnalysis")

process.load('FWCore.MessageService.MessageLogger_cfi')
process.MessageLogger.debugModules = cms.untracked.vstring('analysis')
process.MessageLogger.cerr.threshold = 'DEBUG'
process.MessageLogger.categories.append('Analysis')
process.MessageLogger.cerr.DEBUG = cms.untracked.PSet(
    default = cms.untracked.PSet( limit = cms.untracked.int32(0)),
    Analysis = cms.untracked.PSet( limit = cms.untracked.int32(-1))
)

process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('file:/tmp/antoniov/ExHuME_CEPDijetsGG_M100_10TeV_cff_py_RAW2DIGI_RECO_1.root')
)

process.analysis = cms.EDAnalyzer("ExclusiveDijetsEdmDumpAnalyzer",
    JetTag = cms.InputTag("L2L3CorJetSC7PF"),
    PtMinJet = cms.double(50.0),
    EtaMaxJet = cms.double(2.5),
    DeltaEtaMax = cms.double(999.0),
    DeltaPhiMax = cms.double(999.0),
    NTracksMax = cms.uint32(5),
    NHFPlusMax = cms.uint32(1),
    NHFMinusMax = cms.uint32(1),
    HFThresholdIndex = cms.uint32(10)
)

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("analysisDijets_histos.root")
)

process.analysis_AvePU = cms.Path(process.analysis)
