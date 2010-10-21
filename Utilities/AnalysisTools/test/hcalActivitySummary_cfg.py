import FWCore.ParameterSet.Config as cms

process = cms.Process("Analysis")

process.load('FWCore.MessageService.MessageLogger_cfi')
process.MessageLogger.cerr.threshold = 'INFO'

process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1000) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('file:/data1/antoniov/MinimumBias_Commissioning10_GOODCOLL-Jun14thSkim_v1_RECO/MinimumBias_Commissioning10_GOODCOLL-Jun14thSkim_v1_RECO_EC45524A-E682-DF11-B8A7-001A92810AAA.root')
)

process.output = cms.OutputModule("PoolOutputModule",
    outputCommands = cms.untracked.vstring('drop *','keep *_*_*_Analysis'),
    fileName = cms.untracked.string('hcalActivitySummary.root'),
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('reco_step')
    )
)

process.TFileService = cms.Service("TFileService",
    fileName = cms.string("analysis_histos.root")
)

process.load('Utilities.AnalysisTools.hcalActivitySummary_cfi')
process.load('Utilities.AnalysisTools.hcalActivityFilter_cfi')
process.hcalActivityFilter.EnergyThresholdHB = 1.25
process.hcalActivityFilter.EnergyThresholdHE = 1.9
process.hcalActivityFilter.EnergyThresholdHF = 4.0
#process.hcalActivityFilter.EnergyThresholdEB = 0.6
#process.hcalActivityFilter.EnergyThresholdEE = 2.45
process.hcalActivityFilter.NTowersMaxHFPlus = 0
process.load('Utilities.AnalysisTools.trackHistos_cfi')
process.load('Utilities.AnalysisTools.trackHistoAnalyzer_cfi')

process.reco_step = cms.Path(process.hcalActivitySummary)
process.analysis_step = cms.Path(process.hcalActivityFilter + process.trackHistos + process.trackHistoAnalyzer)
process.out_step = cms.EndPath(process.output)
