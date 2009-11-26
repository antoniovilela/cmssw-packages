import FWCore.ParameterSet.Config as cms

process = cms.Process("ExclusiveDijetsAnalysis")

process.load('ExclusiveDijetsAnalysis.ExclusiveDijetsAnalysis.messageLogger_cfi')
process.MessageLogger.cerr.threshold = 'WARNING'

process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

#from ExclusiveDijetsAnalysis.ExclusiveDijetsAnalysis.filesCEPGGM100_edmDump_noPU_v3_cfi import filesPSet
#from ExclusiveDijetsAnalysis.ExclusiveDijetsAnalysis.filesSDDiJetsPlusPt30_edmDump_cfi import filesPSet
#from ExclusiveDijetsAnalysis.ExclusiveDijetsAnalysis.filesSDDiJetsPlusPt30_FastSim_edmDump_cfi import filesPSet
process.source = cms.Source("PoolSource",
    #fileNames = filesPSet.fileNames
    #fileNames = cms.untracked.vstring('file:/data1/antoniov/edmDump_exclusiveDijets_CEPDijetsGG_M100_10TeV_CMSSW22X.root')
    fileNames = cms.untracked.vstring('file:/data1/antoniov/edmDump_exclusiveDijets-HLTDiJetAve30_PomWigDiJetsPlusPt30.root')
)

from exclusiveDijetsTTreeAnalysis_cfg import process as processTTreeAnalysis
process.exclusiveDijetsTTreeAnalysis = processTTreeAnalysis.exclusiveDijetsTTreeAnalysis
process.exclusiveDijetsTTreeAnalysis.AccessPileUpInfo = False
process.singleVertexFilter = processTTreeAnalysis.singleVertexFilter 
process.exclusiveDijetsFilter = processTTreeAnalysis.exclusiveDijetsFilter
process.exclusiveDijetsSelection = processTTreeAnalysis.exclusiveDijetsSelection

from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.analysisTools import *
makeAnalysis(process,'exclusiveDijetsTTreeAnalysis','exclusiveDijetsSelection')

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("analysisDijets_TTree_noPU.root")
                                   #fileName = cms.string("analysisDijets_TTree_noPU_FastSim.root")
)
