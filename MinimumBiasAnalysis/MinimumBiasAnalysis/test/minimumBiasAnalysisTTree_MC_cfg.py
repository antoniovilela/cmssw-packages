import FWCore.ParameterSet.Config as cms

from minimumBiasAnalysisTTree_cfg import process

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring()
)
from fileNames_MinBias_STARTUP3X_V8P_900GeV_Jan29ReReco import fileNames
process.source.fileNames = fileNames
process.GlobalTag.globaltag = 'STARTUP3X_V8P::All'
process.minimumBiasTTreeAnalysis_eventSelectionMinBiasBSCOR.AccessMCInfo = True
process.minimumBiasTTreeAnalysis_eventSelectionMinBiasPixel.AccessMCInfo = True

for pathname in process.paths:
    getattr(process,pathname).remove(getattr(process,'bptx'))

process.TFileService.fileName = "analysisMinBias_TTree_MinBias.root"
