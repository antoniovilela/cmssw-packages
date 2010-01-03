import FWCore.ParameterSet.Config as cms

from minimumBiasAnalysis_cfg import process
#process.source.duplicateCheckMode = cms.untracked.string('noDuplicateCheck')
process.GlobalTag.globaltag = 'STARTUP3X_V8K::All'
#process.GlobalTag.globaltag = 'STARTUP3X_V8L::All'
process.output.fileName = 'minimumBias.root'

process.output.SelectEvents.SelectEvents = cms.vstring('*')
del process.selection_step
#process.eventSelection_new = cms.Sequence(process.primaryVertexFilter)
#process.selection_step.replace(process.eventSelection,process.eventSelection_new)

#process.reco_step = cms.Path(process.recoSequence+process.BeamHaloId)
