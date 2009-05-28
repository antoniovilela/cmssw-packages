import FWCore.ParameterSet.Config as cms

process = cms.Process("Analysis")

process.load('FWCore.MessageService.MessageLogger_cfi')
process.MessageLogger.debugModules = cms.untracked.vstring('analysis')
process.MessageLogger.cerr.threshold = 'INFO'
process.MessageLogger.cerr.INFO.limit = -1

process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('')
)

#process.load('Configuration/StandardSequences/GeometryPilot2_cff')
#process.load('Configuration/StandardSequences/MagneticField_38T_cff')
#process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
#process.GlobalTag.globaltag = 'IDEAL_V11::All'

#process.load('Configuration/StandardSequences/L1TriggerDefaultMenu_cff')
#from HLTrigger.Configuration.HLT_2E30_cff import hltL1sDiJetAve30,HLTRecoJetMETSequence,hltdijetave30
#process.hltL1sDiJetAve30 = hltL1sDiJetAve30
#process.HLTRecoJetMETSequence = HLTRecoJetMETSequence
#process.hltdijetave30 = hltdijetave30
#process.load('HLTrigger.Configuration.HLT_2E30_cff')

#process.hlt_step = cms.Path(process.hltL1sDiJetAve30 + process.HLTRecoJetMETSequence + process.hltdijetave30)

process.load("ExclusiveDijetsAnalysis.ExclusiveDijetsAnalysis.exclusiveDijetsHLTPaths_cfi")
process.load("DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.pileUpInfo_cfi")
from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.pileUpNumberFilter_cfi import *
#process.filter0PU = pileUpNumberFilter.clone(NumberOfPileUpEvents = 0)
filtersPU = []
for i in range(5):
    filtersPU.append('filter%dPU'%i)
    setattr(process,'filter%dPU'%i,pileUpNumberFilter.clone(NumberOfPileUpEvents = i))

jetTriggers = ['L1_SingleJet30','L1_SingleJet50']
singleEGTriggers = ['L1_SingleEG2','L1_SingleEG5','L1_SingleEG8',
                    'L1_SingleIsoEG5','L1_SingleIsoEG8']
doubleEGTriggers = ['L1_DoubleEG1','L1_DoubleEG5',
                    'L1_DoubleIsoEG8']

allTriggers = jetTriggers[:]
allTriggers += singleEGTriggers
allTriggers += doubleEGTriggers

process.triggerAnalysis = cms.EDAnalyzer("DijetsTriggerAnalyzer",
  GTDigisTag = cms.InputTag("hltGtDigis"),
  GCTDigisTag = cms.InputTag("hltGctDigis"), 
  L1GTObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
  #bit counts per ring
  HFRingETSumThreshold = cms.int32(0),
  AccessL1GctHFRingEtSums = cms.untracked.bool(True),
  L1TriggerNames = cms.vstring(allTriggers) 
)

process.l1filter = cms.EDFilter("L1TriggerTestFilter",
  HFRingETSumThreshold = cms.int32(0),
  L1TriggerNames = cms.vstring("L1_SingleJet30"),
  AccessL1GctHFRingEtSums = cms.untracked.bool(True) 
)

filtersL1 = []
for trig in allTriggers:
    filterName = 'l1filter' + trig
    setattr(process,filterName,process.l1filter.clone(L1TriggerNames = cms.vstring(trig)))
    filtersL1.append(filterName)

attributes = {}
from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.analysisTools import *

filters = filtersPU[:]
filters.append('exclusiveDijetsHLTFilter')

process.pileUpInfo_step = cms.Path(process.pileUpInfo)
makeAnalysis(process,'triggerAnalysis',attributes,filters)

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("analysisDijetsTrigger_histos.root")
)

"""
process.analysis0PU_seq = cms.Sequence(process.pileUpInfo*process.filter0PU*process.triggerAnalysis0PU)
process.hlt = cms.Sequence(process.exclusiveDijetsHLTFilter)
process.analysis_step = cms.Path(process.triggerAnalysis)
process.analysis0PU_step = cms.Path(process.pileUpInfo*process.filter0PU*process.triggerAnalysis0PU)
process.analysisHLT_step = cms.Path(process.hlt+process.triggerAnalysisHLT)
"""

"""
process.filter = cms.Path(process.l1filter)
process.filter_hlt = cms.Path(process.hlt + process.l1filterHLT)
process.filter_0PU = cms.Path(process.pileUpInfo*process.filter0PU*process.l1filter0PU)
"""
