import FWCore.ParameterSet.Config as cms

from dijetsAnalysis_noPU_FastSim_cfg import process
del process.out_step
del process.analysis_step

process.source.fileNames = cms.untracked.vstring('file:/tmp/antoniov/QCDpt30_Summer08_IDEAL_V11_redigi_v1_RECO_345194AC-4EE3-DD11-9EF6-001D0967D634.root')

process.load("DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.singleVertexFilter_cfi")
process.singleVtx = cms.Sequence(process.hlt+process.singleVertexFilter)

from exclusiveDijetsEdmDumpAnalysis_cfg import process as processEdmDumpAnalysis,attributes
process.edmDumpAnalysis = processEdmDumpAnalysis.edmDumpAnalysis
process.edmDumpAnalysis.HFThresholdIndex = 10

from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.analysisTools import *
makeAnalysis(process,'edmDumpAnalysis','hlt',attributes)
makeAnalysis(process,'edmDumpAnalysis','singleVtx',attributes)

process.TFileService.fileName = cms.string("analysisDijets_histos_noPU.root")
