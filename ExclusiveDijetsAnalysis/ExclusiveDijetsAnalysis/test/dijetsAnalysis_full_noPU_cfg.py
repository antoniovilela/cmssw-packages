import FWCore.ParameterSet.Config as cms

# Skim sequences
from dijetsAnalysis_noPU_cfg import process
if hasattr(process,'output'): del process.output
if hasattr(process,'out_step'): del process.out_step
if hasattr(process,'analysis_step'): del process.analysis_step

process.MessageLogger.cerr.threshold = 'INFO'
process.source.fileNames = cms.untracked.vstring('file:/data1/antoniov/ExHuME_CEPDijetsGG_M100_10TeV_CMSSW22X_cff_py_RAW2DIGI_RECO.root')

# Analysis sequences
process.load("DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.singleVertexFilter_cfi")
process.singleVtx = cms.Sequence(process.hlt+process.singleVertexFilter)

from exclusiveDijetsEdmDumpAnalysis_cfg import process as processEdmDumpAnalysis,attributes
process.edmDumpAnalysis = processEdmDumpAnalysis.edmDumpAnalysis

from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.analysisTools import *
makeAnalysis(process,'edmDumpAnalysis','hlt',attributes)
makeAnalysis(process,'edmDumpAnalysis','singleVtx',attributes)

process.TFileService.fileName = cms.string("analysisDijets_histos_noPU.root")
