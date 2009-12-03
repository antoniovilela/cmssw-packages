import FWCore.ParameterSet.Config as cms

from minimumBiasAnalysis_cfg import process
process.GlobalTag.globaltag = 'STARTUP3X_V8F::All'
process.output.fileName = 'minimumBias.root'
process.output.SelectEvents.SelectEvents = cms.vstring('*')
del process.selection_step
process.reco_step = cms.Path(process.recoSequence+process.BeamHaloId)
