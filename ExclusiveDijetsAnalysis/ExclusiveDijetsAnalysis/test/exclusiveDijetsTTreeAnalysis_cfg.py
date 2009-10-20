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

#process.load("JetMETCorrections.Configuration.L2L3Corrections_Summer08Redigi_cff")

process.load("ExclusiveDijetsAnalysis.ExclusiveDijetsAnalysis.exclusiveDijetsFilter_cfi")
process.load("ExclusiveDijetsAnalysis.ExclusiveDijetsAnalysis.exclusiveDijetsTTreeAnalysis_cfi")

process.load("DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.singleVertexFilter_cfi")
process.exclusiveDijetsSelection = cms.Sequence(process.singleVertexFilter+process.exclusiveDijetsFilter)

"""
from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.pileUpNumberFilter_cfi import pileUpNumberFilter
filtersPU = []
for i in range(5):
    filtersPU.append('filter%dPU'%i)
    setattr(process,'filter%dPU'%i,pileUpNumberFilter.clone(NumberOfPileUpEvents = i))

#filters = ['singleVertexFilter']
filters = [] 
filters.extend(filtersPU)
"""

from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.analysisTools import *
makeAnalysis(process,'exclusiveDijetsTTreeAnalysis','exclusiveDijetsSelection')

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("analysisDijets_TTree.root")
)
