import FWCore.ParameterSet.Config as cms

from ExclusiveDijetsAnalysis.ExclusiveDijetsAnalysis.ExclusiveDijetsAnalysis_cfi import ExclusiveDijetsAnalysis

exclusiveDijetsTTreeAnalysis = cms.EDAnalyzer("ExclusiveDijetsTTreeAnalyzer",
    ExclusiveDijetsAnalysis,
    # Saves or not ROOT TTree
    SaveROOTTree = cms.untracked.bool(True)
)
