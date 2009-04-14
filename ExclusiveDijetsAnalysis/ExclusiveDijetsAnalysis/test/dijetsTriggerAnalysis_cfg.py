import FWCore.ParameterSet.Config as cms

process = cms.Process("Analysis")

process.load('FWCore.MessageService.MessageLogger_cfi')
process.MessageLogger.debugModules = cms.untracked.vstring('analysis')
process.MessageLogger.cerr.threshold = 'INFO'
process.MessageLogger.cerr.INFO.limit = -1

process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('file:/tmp/antoniov/ExHuME_CEPDijetsGG_M100_10TeV_cff_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_PU_1.root')
    #fileNames = cms.untracked.vstring('file:/tmp/antoniov/ExHuME_CEPDijetsGG_M100_10TeV_cff_py_RAW2DIGI_RECO_1.root')
)

#process.load("ExclusiveDijetsAnalysis.ExclusiveDijetsAnalysis.exclusiveDijetsHLTPaths_cfi")

process.load('Configuration/StandardSequences/GeometryPilot2_cff')
#process.load('Configuration/StandardSequences/MagneticField_38T_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = 'IDEAL_V11::All'

process.load('Configuration/StandardSequences/L1TriggerDefaultMenu_cff')
#from HLTrigger.Configuration.HLT_2E30_cff import hltL1sDiJetAve30,HLTRecoJetMETSequence,hltdijetave30
#process.hltL1sDiJetAve30 = hltL1sDiJetAve30
#process.HLTRecoJetMETSequence = HLTRecoJetMETSequence
#process.hltdijetave30 = hltdijetave30
process.load('HLTrigger.Configuration.HLT_2E30_cff')

process.load("DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.pileUpInfo_cfi")
from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.pileUpNumberFilter_cfi import *
process.filter0PU = pileUpNumberFilter.clone(NumberOfPileUpEvents = 0)

process.analysis = cms.EDAnalyzer("DijetsTriggerAnalyzer",
  #bit counts per ring
  HFRingETSumThreshold = cms.int32(0),
  AccessL1GctHFRingEtSums = cms.untracked.bool(True)
)

process.analysis0PU = process.analysis.clone()

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("analysisDijetsTrigger_histos.root")
)

process.analysis0PU_seq = cms.Sequence(process.pileUpInfo*process.filter0PU*process.analysis0PU)

#process.hlt_step = cms.Path(process.hltL1sDiJetAve30 + process.HLTRecoJetMETSequence + process.hltdijetave30)

#process.hlt = cms.Sequence(process.exclusiveDijetsHLTFilter)
#process.analysis_step = cms.Path(process.hlt+process.analysis+process.analysis0PU_seq)

process.analysis_step = cms.Path(process.analysis+process.analysis0PU_seq)

#process.schedule = cms.Schedule(process.HLTriggerFirstPath,process.HLT_DiJetAve30,process.analysis_step)
process.schedule = cms.Schedule(process.HLT_DiJetAve30,process.analysis_step)
