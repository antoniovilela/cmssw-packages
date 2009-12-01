import FWCore.ParameterSet.Config as cms

import copy
from HLTrigger.HLTfilters.hltHighLevel_cfi import *
minimumBiasHLTFilter = copy.deepcopy(hltHighLevel)
minimumBiasHLTFilter.HLTPaths = [
    'HLT_MinBias',
    'HLT_MinBiasBSC',
    'HLT_MinBiasBSC_OR',
    'HLT_L1_BSC',
    'HLT_HighMultiplicityBSC',
    'HLT_SplashBSC',
    'HLT_MinBiasEcal',
    'HLT_MinBiasPixel_SingleTrack'
]
