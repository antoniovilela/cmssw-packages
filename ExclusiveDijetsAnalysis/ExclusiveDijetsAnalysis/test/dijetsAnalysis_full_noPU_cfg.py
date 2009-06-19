import FWCore.ParameterSet.Config as cms

process = cms.Process("Analysis")

process.load('FWCore.MessageService.MessageLogger_cfi')
process.MessageLogger.debugModules = cms.untracked.vstring('')
process.MessageLogger.cerr.threshold = 'WARNING'
#process.MessageLogger.cerr.INFO.limit = -1

process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('file:/tmp/antoniov/QCD100to250-madgraph_IDEAL_V12_FastSim_GEN-SIM-DIGI-RECO_FCE6F1F2-FE1C-DE11-9095-0019B9CB01A7.root')
)

process.load("RecoJets.Configuration.RecoPFJets_cff")
process.load("ExclusiveDijetsAnalysis.ExclusiveDijetsAnalysis.analysisSequences_cff")

process.selection_step = cms.Path(process.hlt)
process.reco_step = cms.Path(process.sisCone7PFJets*process.jets*process.tracks*process.edmDump)
#process.analysis_step = cms.Path(process.analysisBefore+process.analysisAfter)

process.load("ExclusiveDijetsAnalysis.ExclusiveDijetsAnalysis.edmDumpAnalysis_cfi")
process.load("DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.singleVertexFilter_cfi")
process.singleVtx = cms.Sequence(process.hlt+process.singleVertexFilter)

attributes = [{'NHFPlusMax':0,'NHFMinusMax':0},
              {'NHFPlusMax':1,'NHFMinusMax':1},
              {'NHFPlusMax':2,'NHFMinusMax':2},
              {'NHFPlusMax':3,'NHFMinusMax':3},
              {'NHFPlusMax':4,'NHFMinusMax':4},
              {'NHFPlusMax':5,'NHFMinusMax':5},
              {'HFThresholdIndex':10},
              {'HFThresholdIndex':12},
              {'HFThresholdIndex':14},
              {'HFThresholdIndex':16}]

from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.analysisTools import *
makeAnalysis(process,'edmDumpAnalysis','hlt',attributes)
makeAnalysis(process,'edmDumpAnalysis','singleVtx',attributes)

#from ExclusiveDijetsAnalysis.ExclusiveDijetsAnalysis.myEventContent_cff import MyEventContent_PAT as MyEventContent
#process.load("ExclusiveDijetsAnalysis.ExclusiveDijetsAnalysis.outputModule_cfi")
#process.output.outputCommands = MyEventContent.outputCommands
#process.output.fileName = '/tmp/antoniov/edmDump_exclusiveDijets.root'
#process.output.SelectEvents.SelectEvents = cms.vstring('selection_step')

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("analysis_histos.root")
)

#process.out_step = cms.EndPath(process.output)
#process.schedule = cms.Schedule(process.hlt_step,process.reco_step,process.analysis_step,process.out_step)
