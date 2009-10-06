import FWCore.ParameterSet.Config as cms

from dijetsAnalysis_cfg import process

process.MessageLogger.cerr.threshold = 'INFO'

#process.recoSequence_new = cms.Sequence(process.jets*process.btagging*process.tracks*process.edmDump)
process.recoSequence_new = cms.Sequence(process.jets*process.tracks*process.edmDump)
process.reco_step.replace(process.recoSequence,process.recoSequence_new) 

process.source.fileNames = cms.untracked.vstring('file:ExHuME_CEPDijetsGG_M100_10TeV_CMSSW22X_cff_py_RAW2DIGI_RECO_41.root')
process.output.fileName = 'edmDump_exclusiveDijets.root'
