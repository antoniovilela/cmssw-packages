import FWCore.ParameterSet.Config as cms

process = cms.Process("Analysis")

process.load("FWCore.MessageLogger.MessageLogger_cfi")
#process.MessageLogger.cerr.FwkReport.reportEvery = 1000 

process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
)

#process.load("Geometry.CMSCommonData.cmsIdealGeometryXML_cfi")
#process.load("Geometry.CaloEventSetup.CaloTowerConstituents_cfi")

#process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
#process.GlobalTag.globaltag = ""
#process.prefer("GlobalTag")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(50000)
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('file:/data1/antoniov/MinBias_Summer08_IDEAL_V11_redigi_v1_GEN-SIM-RECO_FE5226D6-F9CE-DD11-9000-001BFCDBD15E.root')
)
from fileNames_MinimumBias_Jan29ReReco_124023 import fileNames as fileNamesMinimumBias
process.source.fileNames = fileNamesMinimumBias

process.TFileService = cms.Service("TFileService",
    fileName = cms.string('analysisHistos_MinimumBias.root')
)

process.calotwranalysis = cms.EDAnalyzer("CaloTowerAnalyzer",
    CaloTowersLabel = cms.InputTag("towerMaker"),
    AccessRecHits = cms.untracked.bool(True),
    HFRecHitsLabel = cms.untracked.InputTag("hfreco"),
    NBinsHF = cms.untracked.int32(20),
    NumberOfTresholds = cms.uint32(50),
    TowerEnergyTresholdHFMin = cms.double(0.0),
    TowerEnergyTresholdHFMax = cms.double(10.0),
    ReweightHFTower = cms.bool(False),
    #ReweightHistoName = cms.vstring("",""),
    ApplyEnergyOffset = cms.bool(False) 
)

process.load('MinimumBiasAnalysis.MinimumBiasAnalysis.analysisSequences_cff')

#process.analysis = cms.Path(process.eventSelection+process.calotwranalysis)
process.analysis = cms.Path(process.offlineSelection+process.calotwranalysis)
