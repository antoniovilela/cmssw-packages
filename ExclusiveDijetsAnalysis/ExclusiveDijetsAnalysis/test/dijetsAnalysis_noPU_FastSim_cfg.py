import FWCore.ParameterSet.Config as cms

from dijetsAnalysis_cfg import process
#process.maxEvents.input = 5000
process.MessageLogger.cerr.threshold = 'INFO'

process.load("RecoJets.Configuration.RecoPFJets_cff")
#process.recoSequence_new = cms.Sequence(process.jets*process.btagging*process.tracks*process.edmDump)
process.recoSequence_new = cms.Sequence(process.sisCone5PFJets+process.sisCone7PFJets*process.jets*process.tracks*process.edmDump)
process.reco_step.replace(process.recoSequence,process.recoSequence_new) 

process.hfTower.ReweightHFTower = True
process.hfTower.ReweightHistoName = cms.vstring("reweightHistos_MinBias_raw.root","energyHFplusRatio")

process.source.fileNames = cms.untracked.vstring('file:/data1/antoniov/QCD100to250-madgraph_IDEAL_V12_FastSim_GEN-SIM-DIGI-RECO_FCE6F1F2-FE1C-DE11-9095-0019B9CB01A7.root')
process.output.fileName = 'edmDump_exclusiveDijets_FastSim.root'
