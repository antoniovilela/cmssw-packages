import FWCore.ParameterSet.Config as cms

import copy
from HLTrigger.HLTfilters.hltHighLevel_cfi import *
minimumBiasHLTFilter = copy.deepcopy(hltHighLevel)
#minimumBiasHLTFilter.HLTPaths = [
#    'HLT_MinBias',
#    'HLT_MinBiasBSC',
#    'HLT_MinBiasBSC_OR',
#    'HLT_L1_BSC',
#    'HLT_HighMultiplicityBSC',
#    'HLT_SplashBSC',
#    'HLT_MinBiasEcal',
#    'HLT_MinBiasPixel_SingleTrack'
#]
minimumBiasHLTFilter.HLTPaths = ['HLT_PhysicsDeclared']

#from HLTrigger.HLTfilters.hltHighLevelDev_cfi import hltHighLevelDev as minimumBiasHLTFilter
#minimumBiasHLTFilter.TriggerResultsTag = cms.InputTag("TriggerResults","","HLT")
#minimumBiasHLTFilter.HLTPaths = (
#   'HLT_PhysicsDeclared',
#    )
