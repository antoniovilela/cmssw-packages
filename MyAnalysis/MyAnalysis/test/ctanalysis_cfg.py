import FWCore.ParameterSet.Config as cms

process = cms.Process("Analysis")
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1000 

#process.load("Geometry.CMSCommonData.cmsIdealGeometryXML_cfi")
#process.load("Geometry.CaloEventSetup.CaloTowerConstituents_cfi")

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
#process.GlobalTag.connect = "frontier://PromptProd/CMS_COND_21X_GLOBALTAG"
process.GlobalTag.globaltag = "CRUZET4_V4P::All"
process.prefer("GlobalTag")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('file:/tmp/antoniov/POMWIG_SingleDiffractiveDijetsPlus_10TeV_Pt_80_cff_py_RAW2DIGI_RECO_2.root')
)

#/RelValSingleMuPt100/CMSSW_2_1_6_IDEAL_V9_v1/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO
#process.source.fileNames = cms.untracked.vstring('/store/relval/CMSSW_2_1_6/RelValSingleMuPt100/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/IDEAL_V9_v1/0000/7C85FE46-C078-DD11-9335-001D09F241B4.root',
#        '/store/relval/CMSSW_2_1_6/RelValSingleMuPt100/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/IDEAL_V9_v1/0000/B261B730-C078-DD11-B188-001D09F2525D.root',
#        '/store/relval/CMSSW_2_1_6/RelValSingleMuPt100/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/IDEAL_V9_v1/0000/C89D8F1A-C078-DD11-B936-001D09F24D4E.root',
#        '/store/relval/CMSSW_2_1_6/RelValSingleMuPt100/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/IDEAL_V9_v1/0000/FA154F51-C078-DD11-B42F-001D09F2432B.root',
#        '/store/relval/CMSSW_2_1_6/RelValSingleMuPt100/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/IDEAL_V9_v1/0001/90CBF37C-EA78-DD11-B87D-001617DF785A.root'
#)

#/RelValSingleMuPt100/CMSSW_2_1_4_IDEAL_V6_v1/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO
#process.source.fileNames = cms.untracked.vstring('/store/relval/CMSSW_2_1_4/RelValSingleMuPt100/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/IDEAL_V6_v1/0004/800D44DB-2A6C-DD11-9BD6-000423D99EEE.root',
#        '/store/relval/CMSSW_2_1_4/RelValSingleMuPt100/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/IDEAL_V6_v1/0004/B65AA3BC-2A6C-DD11-BCF6-001617C3B77C.root',
#        '/store/relval/CMSSW_2_1_4/RelValSingleMuPt100/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/IDEAL_V6_v1/0004/E4696611-2B6C-DD11-B826-001617C3B654.root',
#        '/store/relval/CMSSW_2_1_4/RelValSingleMuPt100/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/IDEAL_V6_v1/0005/741EBEB6-AC6C-DD11-9762-001617C3B70E.root'
#)

#/RelValSingleMuPt100/CMSSW_2_1_8_IDEAL_V9_v1/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO
#process.source.fileNames = cms.untracked.vstring('/store/relval/CMSSW_2_1_8/RelValSingleMuPt100/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/IDEAL_V9_v1/0002/5AB72DB6-5D82-DD11-A6FE-001617C3B66C.root',
#        '/store/relval/CMSSW_2_1_8/RelValSingleMuPt100/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/IDEAL_V9_v1/0002/781FD5A6-5D82-DD11-982B-001617E30F50.root',
#        '/store/relval/CMSSW_2_1_8/RelValSingleMuPt100/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/IDEAL_V9_v1/0002/D60557C2-5D82-DD11-967E-000423D94AA8.root',
#        '/store/relval/CMSSW_2_1_8/RelValSingleMuPt100/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/IDEAL_V9_v1/0003/A88B53F2-A682-DD11-8E28-000423D6B358.root'
#)

process.TFileService = cms.Service("TFileService",
    fileName = cms.string('analysisHistosRun62232.root')
)

process.calotwranalysis = cms.EDAnalyzer("CaloTowerAnalyzer",
    CaloTowersLabel = cms.InputTag("towerMaker"),
    NBinsHF = cms.untracked.int32(100),
    NumberOfTresholds = cms.uint32(20),
    TowerEnergyTresholdHFMin = cms.double(0.0),
    TowerEnergyTresholdHFMax = cms.double(5.0) 
)

process.analysis = cms.Path(process.calotwranalysis)
