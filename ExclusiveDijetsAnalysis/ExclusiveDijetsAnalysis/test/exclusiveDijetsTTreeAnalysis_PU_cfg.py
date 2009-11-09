import FWCore.ParameterSet.Config as cms

process = cms.Process("EdmDumpAnalysis")

process.load('ExclusiveDijetsAnalysis.ExclusiveDijetsAnalysis.messageLogger_cfi')
process.MessageLogger.cerr.threshold = 'INFO'

process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

from ExclusiveDijetsAnalysis.ExclusiveDijetsAnalysis.filesCEPGGM100_edmDump_cfi import filesPSet
process.source = cms.Source("PoolSource",
    #fileNames = filesPSet.fileNames
    fileNames = cms.untracked.vstring('file:/data1/antoniov/edmDump_exclusiveDijets_CEPDijetsGG_M100_10TeV_StageA156Bx_PU.root')
)

from exclusiveDijetsTTreeAnalysis_cfg import process as processTTreeAnalysis
process.exclusiveDijetsTTreeAnalysis = processTTreeAnalysis.exclusiveDijetsTTreeAnalysis
process.exclusiveDijetsTTreeAnalysis.AccessPileUpInfo = True
process.exclusiveDijetsSelection = processTTreeAnalysis.exclusiveDijetsSelection

from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.analysisTools import *
makeAnalysis(process,'exclusiveDijetsTTreeAnalysis','exclusiveDijetsSelection')

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("analysisDijets_TTree_PU.root")
)
