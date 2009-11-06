import FWCore.ParameterSet.Config as cms

process = cms.Process("Analysis")

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1000 

process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
)

#process.load("Geometry.CMSCommonData.cmsIdealGeometryXML_cfi")
#process.load("Geometry.CaloEventSetup.CaloTowerConstituents_cfi")

#process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
#process.GlobalTag.globaltag = ""
#process.prefer("GlobalTag")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.source = cms.Source("PoolSource",
    #fileNames = cms.untracked.vstring('file:/data1/antoniov/ExHuME_CEPDijetsGG_M100_10TeV_CMSSW22X_cff_py_RAW2DIGI_RECO.root')
    fileNames = cms.untracked.vstring('file:/data1/antoniov/QCD100to250-madgraph_IDEAL_V11_redigi_v1_GEN-SIM-RECO_383EB3A2-3A0A-DE11-82D9-001731A28BE1.root')
)

process.TFileService = cms.Service("TFileService",
    fileName = cms.string('analysisHistos_QCD.root')
)

process.calotwranalysis = cms.EDAnalyzer("CaloTowerAnalyzer",
    CaloTowersLabel = cms.InputTag("towerMaker"),
    AccessRecHits = cms.untracked.bool(True),
    HFRecHitsLabel = cms.untracked.InputTag("hfreco"),
    NBinsHF = cms.untracked.int32(100),
    NumberOfTresholds = cms.uint32(100),
    TowerEnergyTresholdHFMin = cms.double(0.0),
    TowerEnergyTresholdHFMax = cms.double(10.0),
    ReweightHFTower = cms.bool(False),
    #ReweightHistoName = cms.vstring("",""),
    ApplyEnergyOffset = cms.bool(False) 
)

process.analysis = cms.Path(process.calotwranalysis)
