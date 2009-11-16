import FWCore.ParameterSet.Config as cms

from dijetsAnalysis_cfg import process

process.MessageLogger.cerr.threshold = 'INFO'

process.load("DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.pileUpInfo_cfi")
process.pileUpInfo.AccessCrossingFramePlayBack = True
process.pileUpInfo.BunchCrossings = cms.vint32(0)

process.recoSequence_new = cms.Sequence(process.jets*process.tracks*process.edmDump+process.pileUpInfo)
process.reco_step.replace(process.recoSequence,process.recoSequence_new) 

#process.hfTower.ReweightHFTower = False
#process.hfTower.ReweightHistoName = cms.vstring("reweightHistos_MinBias_raw.root","energyHFplusRatio")

process.source.fileNames = cms.untracked.vstring('file:/data1/antoniov/ExHuME_CEPDijetsGG_M100_10TeV_cff_py_RAW2DIGI_RECO_StageA156Bx_PU.root')
process.output.fileName = 'exclusiveDijets_PU.root'
