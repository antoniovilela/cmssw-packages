import FWCore.ParameterSet.Config as cms

process = cms.Process("Analysis")

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1000
process.MessageLogger.cerr.threshold = 'INFO'
#process.MessageLogger.categories.append('Analysis')
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
    #fileNames = cms.untracked.vstring('file:/data1/antoniov/POMWIG_SingleDiffractiveDijetsPlus_10TeV_Pt_30_cff_py_GEN_FASTSIM.root')
    #fileNames = cms.untracked.vstring('file:/data1/antoniov/QCD100to250-madgraph_IDEAL_V12_FastSim_GEN-SIM-DIGI-RECO_FCE6F1F2-FE1C-DE11-9095-0019B9CB01A7.root')
    fileNames = cms.untracked.vstring('file:/data1/antoniov/MinBias_Winter09_IDEAL_V11_FastSim_v1_GEN-SIM-DIGI-RECO_AC464DD4-41CF-DD11-B80D-001E6878F906.root')
)

process.TFileService = cms.Service("TFileService",
    #fileName = cms.string('analysisHistos_QCD_FastSim_offset_reweighted.root')
    #fileName = cms.string('analysisHistos_SDPlusDijets_FastSim_raw_reweighted.root')
    fileName = cms.string('analysisHistos_FastSim.root')
)

process.RandomNumberGeneratorService = cms.Service("RandomNumberGeneratorService",
    calotwranalysis = cms.PSet(
        initialSeed = cms.untracked.uint32(12345),
        engineName = cms.untracked.string('HepJamesRandom')
    ),
    calotwranalysis_offset = cms.PSet(
        initialSeed = cms.untracked.uint32(12345),
        engineName = cms.untracked.string('HepJamesRandom')
    ),
    calotwranalysis_raw_reweight = cms.PSet(
        initialSeed = cms.untracked.uint32(12345),
        engineName = cms.untracked.string('HepJamesRandom')
    ),
    calotwranalysis_offset_reweight = cms.PSet(
        initialSeed = cms.untracked.uint32(12345),
        engineName = cms.untracked.string('HepJamesRandom')
    )
)

process.calotwranalysis = cms.EDAnalyzer("CaloTowerAnalyzer",
    CaloTowersLabel = cms.InputTag("towerMaker"),
    AccessRecHits = cms.untracked.bool(False),
    HFRecHitsLabel = cms.untracked.InputTag("caloRecHits"),
    NBinsHF = cms.untracked.int32(100),
    NumberOfTresholds = cms.uint32(100),
    TowerEnergyTresholdHFMin = cms.double(0.0),
    TowerEnergyTresholdHFMax = cms.double(10.0),
    ApplyEnergyOffset = cms.bool(False),
    EnergyOffsetParameters = cms.PSet(
        meanHFEnergy = cms.double(0.60),
        sigmaHFEnergy = cms.double(0.30)
    ),
    ReweightHFTower = cms.bool(False),
    ReweightHistoName = cms.vstring("","")  
)
process.calotwranalysis_offset = process.calotwranalysis.clone()
process.calotwranalysis_offset.ApplyEnergyOffset = True

process.calotwranalysis_raw_reweight = process.calotwranalysis.clone()
process.calotwranalysis_raw_reweight.ReweightHFTower = True
#process.calotwranalysis_raw_reweight.ReweightHistoName = cms.vstring("reweightHistos_MinBias_raw.root","energyHFplusRatio")
process.calotwranalysis_raw_reweight.ReweightHistoName = cms.vstring("reweightHistos_MinBias_raw.root","energyHFRatioAverage")

process.calotwranalysis_offset_reweight = process.calotwranalysis.clone()
process.calotwranalysis_offset_reweight.ApplyEnergyOffset = True 
process.calotwranalysis_offset_reweight.ReweightHFTower = True
#process.calotwranalysis_offset_reweight.ReweightHistoName = cms.vstring("reweightHistos_MinBias_offset.root","energyHFRatioAverage")

#process.analysis = cms.Path(process.calotwranalysis+
#                            process.calotwranalysis_offset)

process.analysis = cms.Path(process.calotwranalysis+
                            process.calotwranalysis_raw_reweight)

#process.analysis = cms.Path(process.calotwranalysis+
#                            process.calotwranalysis_offset+
#                            process.calotwranalysis_raw_reweight+
#                            process.calotwranalysis_offset_reweight)
