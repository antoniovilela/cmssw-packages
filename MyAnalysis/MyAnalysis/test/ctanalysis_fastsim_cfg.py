import FWCore.ParameterSet.Config as cms

process = cms.Process("Analysis")

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1000
#rocess.MessageLogger.cerr.threshold = 'INFO'
#rocess.MessageLogger.categories.append('Analysis')
#process.MessageLogger.cerr.INFO = cms.untracked.PSet(
#    default = cms.untracked.PSet( limit = cms.untracked.int32(0)),
#    Analysis = cms.untracked.PSet( limit = cms.untracked.int32(-1))
#)

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
    fileNames = cms.untracked.vstring('file:/data1/antoniov/POMWIG_SingleDiffractiveDijetsPlus_10TeV_Pt_30_cff_py_GEN_FASTSIM.root')
)

process.TFileService = cms.Service("TFileService",
    fileName = cms.string('analysisHistos_FastSim.root')
)

process.calotwranalysis = cms.EDAnalyzer("CaloTowerAnalyzer",
    CaloTowersLabel = cms.InputTag("towerMaker"),
    AccessRecHits = cms.untracked.bool(False),
    HFRecHitsLabel = cms.untracked.InputTag("caloRecHits"),
    NBinsHF = cms.untracked.int32(100),
    NumberOfTresholds = cms.uint32(100),
    TowerEnergyTresholdHFMin = cms.double(0.0),
    TowerEnergyTresholdHFMax = cms.double(10.0),
    ReweightHFTower = cms.bool(True),
    ReweightHistoName = cms.vstring("reweightHisto.root","energyHFplusRatio")  
)

process.analysis = cms.Path(process.calotwranalysis)
