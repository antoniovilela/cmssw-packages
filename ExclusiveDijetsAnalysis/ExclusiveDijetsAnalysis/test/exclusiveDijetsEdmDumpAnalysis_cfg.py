import FWCore.ParameterSet.Config as cms

process = cms.Process("EdmDumpAnalysis")

process.load('ExclusiveDijetsAnalysis.ExclusiveDijetsAnalysis.messageLogger_cfi')
process.MessageLogger.cerr.threshold = 'INFO'

process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

from ExclusiveDijetsAnalysis.ExclusiveDijetsAnalysis.filesCEPGGM100_edmDump_cfi import filesPSet

process.source = cms.Source("PoolSource",
    fileNames = filesPSet.fileNames
    #fileNames = cms.untracked.vstring('file:/tmp/antoniov/edmDump_exclusiveDijets.root')
)

#process.load("JetMETCorrections.Configuration.L2L3Corrections_Summer08Redigi_cff")

process.load("ExclusiveDijetsAnalysis.ExclusiveDijetsAnalysis.edmDumpAnalysis_cfi")
process.edmDumpAnalysis.NHFPlusMax = 0
process.edmDumpAnalysis.NHFMinusMax = 0
process.edmDumpAnalysis.HFThresholdIndex = 14
process.edmDumpAnalysis.UsePAT = True
process.edmDumpAnalysis.JetTag = "selectedLayer1JetsSC7PF"

process.load("DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.singleVertexFilter_cfi")

from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.pileUpNumberFilter_cfi import pileUpNumberFilter
filtersPU = []
for i in range(5):
    filtersPU.append('filter%dPU'%i)
    setattr(process,'filter%dPU'%i,pileUpNumberFilter.clone(NumberOfPileUpEvents = i))

attributes = [{'NHFPlusMax':0,'NHFMinusMax':0},
              {'NHFPlusMax':1,'NHFMinusMax':1},
              {'NHFPlusMax':2,'NHFMinusMax':2},
              {'NHFPlusMax':3,'NHFMinusMax':3},
              {'NHFPlusMax':4,'NHFMinusMax':4},
              {'NHFPlusMax':5,'NHFMinusMax':5},
              {'HFThresholdIndex':7},
              {'HFThresholdIndex':8},
              {'HFThresholdIndex':9},
              {'HFThresholdIndex':10},
              {'HFThresholdIndex':11},
              {'HFThresholdIndex':12},
              {'HFThresholdIndex':13}, 
              {'HFThresholdIndex':14},
              {'HFThresholdIndex':15},
              {'HFThresholdIndex':16},
              {'HFThresholdIndex':17},
              {'HFThresholdIndex':18},
              {'ThirdJetPtMax':10},
              {'ThirdJetPtMax':15},
              {'ThirdJetPtMax':20}, 
              {'DoBTag':True},]

#filters = ['singleVertexFilter']
filters = [] 
filters.extend(filtersPU)

from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.analysisTools import *
makeAnalysis(process,'edmDumpAnalysis','',attributes,filters)
makeAnalysis(process,'edmDumpAnalysis','singleVertexFilter',attributes,filters)

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("analysisDijets_histos_PU.root")
)
