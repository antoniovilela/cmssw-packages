import FWCore.ParameterSet.Config as cms

from MinimumBiasAnalysis.MinimumBiasAnalysis.MinimumBiasAnalysis_cfi import MinimumBiasAnalysis

minimumBiasTTreeAnalysis = cms.EDAnalyzer("MinimumBiasTTreeAnalyzer",
    MinimumBiasAnalysis,
    # Saves or not ROOT TTree
    SaveROOTTree = cms.untracked.bool(True)
)
