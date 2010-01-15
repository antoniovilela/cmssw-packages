import FWCore.ParameterSet.Config as cms

# Skim sequences
from dijetsAnalysis_noPU_FastSim_cfg import process
if hasattr(process,'output'): del process.output
if hasattr(process,'out_step'): del process.out_step
if hasattr(process,'analysis_step'): del process.analysis_step

process.MessageLogger.cerr.threshold = 'WARNING'
#process.maxEvents.input = 5000
process.source.fileNames = cms.untracked.vstring('file:/data1/antoniov/QCD100to250-madgraph_IDEAL_V12_FastSim_GEN-SIM-DIGI-RECO_FCE6F1F2-FE1C-DE11-9095-0019B9CB01A7.root')

# Analysis sequences
process.load("DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.singleVertexFilter_cfi")
process.singleVtx = cms.Sequence(process.hlt+process.singleVertexFilter)

from exclusiveDijetsTTreeAnalysis_cfg import process as processTTreeAnalysis
process.exclusiveDijetsTTreeAnalysis = processTTreeAnalysis.exclusiveDijetsTTreeAnalysis
process.exclusiveDijetsTTreeAnalysis.JetTag = "selectedLayer1Jets"
process.exclusiveDijetsTTreeAnalysis.AccessPileUpInfo = False
process.singleVertexFilter = processTTreeAnalysis.singleVertexFilter 
process.exclusiveDijetsFilter = processTTreeAnalysis.exclusiveDijetsFilter
process.exclusiveDijetsFilter.JetTag = "selectedLayer1Jets"
process.exclusiveDijetsSelection = cms.Sequence(process.hlt+process.singleVertexFilter+process.exclusiveDijetsFilter)

from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.analysisTools import *
makeAnalysis(process,'exclusiveDijetsTTreeAnalysis','exclusiveDijetsSelection')

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("analysisDijets_TTree_noPU_FastSim_full.root")
)
