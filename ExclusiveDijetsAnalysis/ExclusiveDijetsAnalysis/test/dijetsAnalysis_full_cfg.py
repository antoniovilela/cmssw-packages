import FWCore.ParameterSet.Config as cms

from dijetsAnalysis_cfg import process
del process.out_step
del process.analysis_step

process.source.fileNames = cms.untracked.vstring('file:/tmp/antoniov/ExHuME_CEPDijetsGG_M100_10TeV_cff_py_RAW2DIGI_RECO.root')

process.load("DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.singleVertexFilter_cfi")
process.singleVtx = cms.Sequence(process.hlt+process.singleVertexFilter)

from exclusiveDijetsEdmDumpAnalysis_cfg import process as processEdmDumpAnalysis,attributes
process.edmDumpAnalysis = processEdmDumpAnalysis.edmDumpAnalysis

from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.pileUpNumberFilter_cfi import pileUpNumberFilter
filtersPU = []
for i in range(5):
    filtersPU.append('filter%dPU'%i)
    setattr(process,'filter%dPU'%i,pileUpNumberFilter.clone(NumberOfPileUpEvents = i))

filters = [] 
filters.extend(filtersPU)

from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.analysisTools import *
makeAnalysis(process,'edmDumpAnalysis','hlt',attributes,filters)
makeAnalysis(process,'edmDumpAnalysis','singleVtx',attributes,filters)

process.TFileService.fileName = cms.string("analysisDijets_histos_PU.root")
