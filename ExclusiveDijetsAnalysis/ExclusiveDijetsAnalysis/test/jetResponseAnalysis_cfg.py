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

from ExclusiveDijetsAnalysis.ExclusiveDijetsAnalysis.leadingJets_cfi import leadingJets
process.leadingGenJetsSC5 = leadingJets.clone(src = cms.InputTag("sisCone5GenJets"))
process.leadingGenJetsSC7 = leadingJets.clone(src = cms.InputTag("sisCone7GenJets"))

matchGenToRecoJet = cms.EDProducer("TrivialDeltaRViewMatcher",
    src = cms.InputTag(""),
    matched = cms.InputTag(""),
    distMin = cms.double(0.25)
)
process.matchGenToRecoJetSC5Calo = matchGenToRecoJet.clone(src="leadingGenJetsSC5",matched="L2L3CorJetSC5Calo")
process.matchGenToRecoJetSC7Calo = matchGenToRecoJet.clone(src="leadingGenJetsSC7",matched="L2L3CorJetSC7Calo")
process.matchGenToRecoJetSC5PF = matchGenToRecoJet.clone(src="leadingGenJetsSC5",matched="L2L3CorJetSC5PF")
process.matchGenToRecoJetSC7PF = matchGenToRecoJet.clone(src="leadingGenJetsSC7",matched="L2L3CorJetSC7PF")

jetResponseAnalysis = cms.EDAnalyzer("SimpleJetResponseAnalyzer",
   GenToRecoJetMatchTag = cms.InputTag("")
)
process.jetResponseAnalysisSC5Calo = jetResponseAnalysis.clone(GenToRecoJetMatchTag="matchGenToRecoJetSC5Calo")
process.jetResponseAnalysisSC7Calo = jetResponseAnalysis.clone(GenToRecoJetMatchTag="matchGenToRecoJetSC7Calo")
process.jetResponseAnalysisSC5PF = jetResponseAnalysis.clone(GenToRecoJetMatchTag="matchGenToRecoJetSC5PF")
process.jetResponseAnalysisSC7PF = jetResponseAnalysis.clone(GenToRecoJetMatchTag="matchGenToRecoJetSC7PF")

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("jetResponseAnalysis_TTree.root")
)

process.load("ExclusiveDijetsAnalysis.ExclusiveDijetsAnalysis.outputModule_cfi")
process.output.outputCommands = cms.untracked.vstring('drop *','keep *_*_*_Analysis') 
process.output.fileName = '/data1/antoniov/jetResponse_test.root'
#process.output.SelectEvents.SelectEvents = cms.vstring('selection_step')

#process.reco_step = cms.Path(process.L2L3CorJetSC7PF)
process.reco_step = cms.Path(process.L2L3CorJetSC5Calo+process.L2L3CorJetSC7Calo+process.L2L3CorJetSC5PF+process.L2L3CorJetSC7PF)
process.analysis_step = cms.Path((process.leadingGenJetsSC5+process.leadingGenJetsSC7)*
                                 (process.matchGenToRecoJetSC5Calo+process.matchGenToRecoJetSC7Calo+
                                 process.matchGenToRecoJetSC5PF+process.matchGenToRecoJetSC7PF)*
                                 (process.jetResponseAnalysisSC5Calo+process.jetResponseAnalysisSC7Calo+
                                 process.jetResponseAnalysisSC5PF+process.jetResponseAnalysisSC7PF)) 

#process.out_step = cms.EndPath(process.output)
