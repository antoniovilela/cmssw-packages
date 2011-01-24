import FWCore.ParameterSet.Config as cms

process = cms.Process("Analysis")

process.load('FWCore.MessageService.MessageLogger_cfi')
#process.MessageLogger.debugModules = cms.untracked.vstring('analysis')
process.MessageLogger.cerr.threshold = 'INFO'
process.MessageLogger.cerr.INFO.limit = -1

process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('file:')
)

process.load('JetMETCorrections.Configuration.JetCorrectionProducersAllAlgos_cff')

from Utilities.AnalysisSequences.leadingJets_cfi import leadingJets

matchGenToRecoJet = cms.EDProducer("TrivialDeltaRViewMatcher",
    src = cms.InputTag(""),
    matched = cms.InputTag(""),
    distMin = cms.double(0.25)
)

jetResponseAnalysis = cms.EDAnalyzer("SimpleJetResponseAnalyzer",
   GenToRecoJetMatchTag = cms.InputTag("")
)

from Utilities.PyConfigTools.analysisTools import *

jetAlgos = {'ak5':('Calo','PF'),
            'ak7':('Calo','PF')}

for algo in jetAlgos:
    #algoFullName = ((algo.replace('KT','kt')).replace('SC','sisCone')).replace('IC','iterativeCone')
    algoFullName = algo 
    genCollLabel = algoFullName + 'GenJets'
    leadingJetsLabel = 'leadingGenJets' + algo
    if not hasattr(process,leadingJetsLabel):
        setattr(process,leadingJetsLabel,leadingJets.clone(src=genCollLabel))

    for type in jetAlgos[algo]:
        corrCollLabel = algo + type + 'JetsL2L3'
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

process.load("Utilities.AnalysisSequences.outputModule_cfi")
process.output.outputCommands = cms.untracked.vstring('drop *','keep *_*_*_Analysis') 
process.output.fileName = 'jetResponse.root'
#process.output.SelectEvents.SelectEvents = cms.vstring('selection_step')
#process.reco_step = cms.Path()
#process.out_step = cms.EndPath(process.output)
