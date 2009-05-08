import FWCore.ParameterSet.Config as cms

from PhysicsTools.PatAlgos.patLayer0_cff import *
from PhysicsTools.PatAlgos.patLayer1_cff import *

"""
patBeforeLevel0Reco = cms.Sequence(patAODJetMETCorrections)

patLayer0Cleaners = cms.Sequence(allLayer0Jets*
                                 allLayer0METs)

patHighLevelReco = cms.Sequence(patJetFlavourId*
                                patLayer0JetMETCorrections*
                                patLayer0JetTracksCharge)

patMCTruth = cms.Sequence(patMCTruth_Jet)

patLayer0_withoutTrigMatch_new = cms.Sequence(patBeforeLevel0Reco*
                                              patLayer0Cleaners*
                                              patHighLevelReco*
                                              patMCTruth)

patLayer0 = patLayer0_withoutTrigMatch_new

allLayer1Jets.addTrigMatch = False
#allLayer1Jets.trigPrimMatch = cms.VInputTag()
allLayer1METs.addTrigMatch = False

patLayer1 = cms.Sequence(layer1Jets*layer1METs)
"""
