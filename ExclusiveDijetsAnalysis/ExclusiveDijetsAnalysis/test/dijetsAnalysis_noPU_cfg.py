import FWCore.ParameterSet.Config as cms

from dijetsAnalysis_cfg import process

#process.MessageLogger.cerr.threshold = 'INFO'

#process.recoSequence_new = cms.Sequence(process.jets*process.btagging*process.tracks*process.edmDump)
process.recoSequence_new = cms.Sequence(process.jets*process.tracks*process.edmDump)
process.reco_step.replace(process.recoSequence,process.recoSequence_new) 

process.source.fileNames = cms.untracked.vstring('file:/tmp/antoniov/QCDpt30_Summer08_IDEAL_V11_redigi_v1_RECO_345194AC-4EE3-DD11-9EF6-001D0967D634.root')
process.output.fileName = 'edmDump_exclusiveDijets.root'
