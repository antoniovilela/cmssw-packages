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

matchGenToRecoJet = cms.EDProducer("TrivialDeltaRViewMatcher",
    src = cms.InputTag(""),
    matched = cms.InputTag(""),
    distMin = cms.double(0.25)
)

jetResponseAnalysis = cms.EDAnalyzer("SimpleJetResponseAnalyzer",
   GenToRecoJetMatchTag = cms.InputTag("")
)

from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.analysisTools import *

#jetAlgos = ['KT4','KT6','SC5','SC7']
#jetTypes = ['Calo','PF']

jetAlgos = {'IC5':('Calo','PF'),
            'KT4':('Calo','PF'),
            'KT6':('Calo','PF'),
            'SC5':('Calo','PF'),
            'SC7':('Calo','PF')}

collPrefix = 'L2L3CorJet'
for algo in jetAlgos:
    algoFullName = ((algo.replace('KT','kt')).replace('SC','sisCone')).replace('IC','iterativeCone')
    genCollLabel = algoFullName + 'GenJets'
    leadingJetsLabel = 'leadingGenJets' + algo
    if not hasattr(process,leadingJetsLabel):
        setattr(process,leadingJetsLabel,leadingJets.clone(src=genCollLabel))

    for type in jetAlgos[algo]:
        corrCollLabel = collPrefix + algo + type
        matchGenToRecoJetlabel = 'matchGenToRecoJet' + algo + type
        if not hasattr(process,matchGenToRecoJetlabel):
            setattr(process,matchGenToRecoJetlabel,matchGenToRecoJet.clone(src=genCollLabel,matched=corrCollLabel)) 

        jetResponseAnalysisLabel = 'jetResponseAnalysis' + algo + type
        if not hasattr(process,jetResponseAnalysisLabel):
            setattr(process,jetResponseAnalysisLabel,jetResponseAnalysis.clone(GenToRecoJetMatchTag=matchGenToRecoJetlabel))

        sequenceLabel = "analysis_" + algo + type
        if not hasattr(process,sequenceLabel):
            setattr(process,sequenceLabel,cms.Sequence((getattr(process,leadingJetsLabel)+
                                                       getattr(process,corrCollLabel))*
                                                       getattr(process,matchGenToRecoJetlabel)*
                                                       getattr(process,jetResponseAnalysisLabel)))

        print "Adding",sequenceLabel
        pathLabel = sequenceLabel + "_step"  
        if not hasattr(process,pathLabel):
            setattr(process,pathLabel,cms.Path(getattr(process,sequenceLabel)))

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("jetResponseAnalysis_TTree.root")
)

process.load("ExclusiveDijetsAnalysis.ExclusiveDijetsAnalysis.outputModule_cfi")
process.output.outputCommands = cms.untracked.vstring('drop *','keep *_*_*_Analysis') 
process.output.fileName = '/data1/antoniov/jetResponse_test.root'
#process.output.SelectEvents.SelectEvents = cms.vstring('selection_step')

#process.reco_step = cms.Path(process.L2L3CorJetSC7PF)
#process.out_step = cms.EndPath(process.output)
