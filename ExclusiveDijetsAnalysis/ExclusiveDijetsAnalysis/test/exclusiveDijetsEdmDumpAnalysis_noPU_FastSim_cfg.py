import FWCore.ParameterSet.Config as cms

process = cms.Process("EdmDumpAnalysis")

process.load('ExclusiveDijetsAnalysis.ExclusiveDijetsAnalysis.messageLogger_cfi')
process.MessageLogger.cerr.threshold = 'INFO'

process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('file:/tmp/antoniov/edmDump_exclusiveDijets_FastSim.root')
)

from exclusiveDijetsEdmDumpAnalysis_cfg import process as processEdmDumpAnalysis,attributes
process.edmDumpAnalysis = processEdmDumpAnalysis.edmDumpAnalysis
process.edmDumpAnalysis.HFThresholdIndex = 10

process.load("DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.singleVertexFilter_cfi")

from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.analysisTools import *
makeAnalysis(process,'edmDumpAnalysis','',attributes)
makeAnalysis(process,'edmDumpAnalysis','singleVertexFilter',attributes)

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("analysisDijets_histos_noPU.root")
)
