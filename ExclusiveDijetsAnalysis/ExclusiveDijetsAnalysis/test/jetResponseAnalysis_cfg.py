import FWCore.ParameterSet.Config as cms

process = cms.Process("Analysis")

process.load('FWCore.MessageService.MessageLogger_cfi')
#process.MessageLogger.debugModules = cms.untracked.vstring('analysis')
process.MessageLogger.cerr.threshold = 'INFO'
process.MessageLogger.cerr.INFO.limit = -1

process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('file:/data1/antoniov/ExHuME_CEPDijetsGG_M100_10TeV_CMSSW22X_cff_py_RAW2DIGI_RECO.root')
)

#process.load("ExclusiveDijetsAnalysis.ExclusiveDijetsAnalysis.exclusiveDijetsHLTPaths_cfi")

process.load("JetMETCorrections.Configuration.L2L3Corrections_Summer08Redigi_cff")

"""
process.load("DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.pileUpInfo_cfi")
from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.pileUpNumberFilter_cfi import *
filtersPU = []
for i in range(5):
    filtersPU.append('filter%dPU'%i)
    setattr(process,'filter%dPU'%i,pileUpNumberFilter.clone(NumberOfPileUpEvents = i))

attributes = {}
from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.analysisTools import *

filters = filtersPU[:]
filters.append('exclusiveDijetsHLTFilter')

process.pileUpInfo_step = cms.Path(process.pileUpInfo)
makeAnalysis(process,'',attributes,filters)
"""

process.matchGenToRecoJet = cms.EDProducer("TrivialDeltaRViewMatcher",
    src = cms.InputTag("sisCone7GenJets"),
    matched = cms.InputTag("L2L3CorJetSC7PF"),
    distMin = cms.double(0.25)
)

process.jetResponseAnalysis = cms.EDAnalyzer("SimpleJetResponseAnalyzer",
   GenToRecoJetMatchTag = cms.InputTag("matchGenToRecoJet")
)

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("jetResponseAnalysis_TTree.root")
)

process.load("ExclusiveDijetsAnalysis.ExclusiveDijetsAnalysis.outputModule_cfi")
process.output.outputCommands = cms.untracked.vstring('drop *','keep *_*_*_Analysis') 
process.output.fileName = '/data1/antoniov/jetResponse_test.root'
#process.output.SelectEvents.SelectEvents = cms.vstring('selection_step')

process.reco_step = cms.Path(process.L2L3CorJetSC7PF)
process.analysis_step = cms.Path(process.matchGenToRecoJet*process.jetResponseAnalysis)
#process.out_step = cms.EndPath(process.output)
