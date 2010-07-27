import FWCore.ParameterSet.Config as cms

process = cms.Process("Analysis")

process.load('FWCore.MessageService.MessageLogger_cfi')
process.MessageLogger.cerr.threshold = 'INFO'

process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('file:/tmp/antoniov/MinimumBias_Commissioning10_GOODCOLL-Jun14thSkim_v1_CA78CDED-2E83-DF11-BDD8-0026189438A7.root')
)

process.load('Utilities.AnalysisTools.hcalActivitySummary_cfi')

process.output = cms.OutputModule("PoolOutputModule",
    outputCommands = cms.untracked.vstring('drop *','keep *_*_*_Analysis'),
    fileName = cms.untracked.string('/tmp/antoniov/hcalActivitySummary.root'),
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('analysis_step')
    )
)

process.analysis_step = cms.Path(process.hcalActivitySummary)
process.out_step = cms.EndPath(process.output)
