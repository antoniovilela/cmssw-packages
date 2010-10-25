import FWCore.ParameterSet.Config as cms

process = cms.Process("Analysis")

process.load('FWCore.MessageService.MessageLogger_cfi')
process.MessageLogger.cerr.threshold = 'INFO'

process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(3000) )

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
process.hcalActivityFilter.EnergyThresholdEB = 0.6
process.hcalActivityFilter.EnergyThresholdEE = 2.45
process.hcalVetoSumEMaxHFPlus4 = process.hcalActivityFilter.clone(SumEMaxHFPlus = 4.0)
process.hcalVetoSumEMaxHFMinus4 = process.hcalActivityFilter.clone(SumEMaxHFMinus = 4.0)
process.hcalVetoSumETMaxHFPlus1 = process.hcalActivityFilter.clone(SumETMaxHFPlus = 1.0)
process.hcalVetoSumETMaxHFMinus1 = process.hcalActivityFilter.clone(SumETMaxHFMinus = 1.0)
process.hcalVetoSumEMaxHFPlus4SumEMaxHEPlus2 = process.hcalActivityFilter.clone(SumEMaxHFPlus = 4.0, SumEMaxHEPlus = 2.0)
process.hcalVetoSumEMaxHFMinus4SumEMaxHEMinus2 = process.hcalActivityFilter.clone(SumEMaxHFMinus = 4.0, SumEMaxHEMinus = 2.0)
process.hcalVetoSumETMaxHFPlus1SumETMaxHEPlus1 = process.hcalActivityFilter.clone(SumETMaxHFPlus = 1.0, SumETMaxHEPlus = 1.0)
process.hcalVetoSumETMaxHFMinus1SumETMaxHEMinus1 = process.hcalActivityFilter.clone(SumETMaxHFMinus = 1.0, SumETMaxHEMinus = 1.0)

#process.load('Utilities.AnalysisTools.trackHistos_cfi')
process.load('Utilities.AnalysisTools.trackHistoAnalyzer_cfi')
process.trackHistoAnalyzer_hcalVetoSumEMaxHFPlus4 = process.trackHistoAnalyzer.clone()
process.trackHistoAnalyzer_hcalVetoSumEMaxHFMinus4 = process.trackHistoAnalyzer.clone()
process.trackHistoAnalyzer_hcalVetoSumETMaxHFPlus1 = process.trackHistoAnalyzer.clone()
process.trackHistoAnalyzer_hcalVetoSumETMaxHFMinus1 = process.trackHistoAnalyzer.clone()
process.trackHistoAnalyzer_hcalVetoSumEMaxHFPlus4SumEMaxHEPlus2 = process.trackHistoAnalyzer.clone()
process.trackHistoAnalyzer_hcalVetoSumEMaxHFMinus4SumEMaxHEMinus2 = process.trackHistoAnalyzer.clone()
process.trackHistoAnalyzer_hcalVetoSumETMaxHFPlus1SumETMaxHEPlus1 = process.trackHistoAnalyzer.clone()
process.trackHistoAnalyzer_hcalVetoSumETMaxHFMinus1SumETMaxHEMinus1 = process.trackHistoAnalyzer.clone()

process.reco_step = cms.Path(process.hcalActivitySummary)
process.analysis_hcalVetoSumEMaxHFPlus4_step = cms.Path(process.hcalVetoSumEMaxHFPlus4 + process.trackHistoAnalyzer_hcalVetoSumEMaxHFPlus4)
process.analysis_hcalVetoSumEMaxHFMinus4_step = cms.Path(process.hcalVetoSumEMaxHFMinus4 + process.trackHistoAnalyzer_hcalVetoSumEMaxHFMinus4)
process.analysis_hcalVetoSumETMaxHFPlus1_step = cms.Path(process.hcalVetoSumETMaxHFPlus1 + process.trackHistoAnalyzer_hcalVetoSumETMaxHFPlus1)
process.analysis_hcalVetoSumETMaxHFMinus1_step = cms.Path(process.hcalVetoSumETMaxHFMinus1 + process.trackHistoAnalyzer_hcalVetoSumETMaxHFMinus1)
process.analysis_hcalVetoSumEMaxHFPlus4SumEMaxHEPlus2_step = cms.Path(process.hcalVetoSumEMaxHFPlus4SumEMaxHEPlus2 + process.trackHistoAnalyzer_hcalVetoSumEMaxHFPlus4SumEMaxHEPlus2)
process.analysis_hcalVetoSumEMaxHFMinus4SumEMaxHEMinus2_step = cms.Path(process.hcalVetoSumEMaxHFMinus4SumEMaxHEMinus2 + process.trackHistoAnalyzer_hcalVetoSumEMaxHFMinus4SumEMaxHEMinus2)
process.analysis_hcalVetoSumETMaxHFPlus1SumETMaxHEPlus1_step = cms.Path(process.hcalVetoSumETMaxHFPlus1SumETMaxHEPlus1 + process.trackHistoAnalyzer_hcalVetoSumETMaxHFPlus1SumETMaxHEPlus1)
process.analysis_hcalVetoSumETMaxHFMinus1SumETMaxHEMinus1_step = cms.Path(process.hcalVetoSumETMaxHFMinus1SumETMaxHEMinus1 + process.trackHistoAnalyzer_hcalVetoSumETMaxHFMinus1SumETMaxHEMinus1)
process.out_step = cms.EndPath(process.output)
