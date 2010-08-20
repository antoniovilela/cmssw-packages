import FWCore.ParameterSet.Config as cms

process = cms.Process("Analysis")

process.load('FWCore.MessageService.MessageLogger_cfi')
process.MessageLogger.cerr.threshold = 'INFO'
process.MessageLogger.categories.append('LuminosityWeightProducer')
process.MessageLogger.cerr.LuminosityWeightProducer = cms.untracked.PSet(limit = cms.untracked.int32(-1))

process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('file:/tmp/antoniov/MinimumBias_GOODCOLL-Jun14thSkim_v1_5ED097BF-B083-DF11-A9FF-0018F3D09678.root')
)

process.output = cms.OutputModule("PoolOutputModule",
    outputCommands = cms.untracked.vstring('drop *','keep *_*_*_Analysis'),
    fileName = cms.untracked.string('/tmp/antoniov/lumiWeightProducer.root'),
    #SelectEvents = cms.untracked.PSet(
    #    SelectEvents = cms.vstring('')
    #)
)

process.TFileService = cms.Service("TFileService",
    fileName = cms.string("analysis_histos.root")
)

process.lumiWeight = cms.EDProducer("LuminosityWeightProducer",
    rootFileName = cms.string("out_Cert_132440-142664_7TeV_StreamExpress_Collisions10_v2.root"),
    prefix = cms.untracked.string("instLumi")
)

process.lumiAnalyzer = cms.EDAnalyzer("SimpleLumiAnalyzer",
    VertexTag = cms.InputTag("offlinePrimaryVertices"),
    LumiWeightTag = cms.InputTag("lumiWeight")
)

process.reco_step = cms.Path(process.lumiWeight)
process.analysis_step = cms.Path(process.lumiAnalyzer)
process.out_step = cms.EndPath(process.output)
