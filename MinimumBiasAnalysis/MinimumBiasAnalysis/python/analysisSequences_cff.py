import FWCore.ParameterSet.Config as cms

from L1TriggerConfig.L1GtConfigProducers.L1GtTriggerMaskAlgoTrigConfig_cff import *
from L1TriggerConfig.L1GtConfigProducers.L1GtTriggerMaskTechTrigConfig_cff import *
es_prefer_l1GtTriggerMaskAlgoTrig = cms.ESPrefer("L1GtTriggerMaskAlgoTrigTrivialProducer","l1GtTriggerMaskAlgoTrig")
es_prefer_l1GtTriggerMaskTechTrig = cms.ESPrefer("L1GtTriggerMaskTechTrigTrivialProducer","l1GtTriggerMaskTechTrig")
from HLTrigger.HLTfilters.hltLevel1GTSeed_cfi import *
hltLevel1GTSeed.L1TechTriggerSeeding = cms.bool(True)
#hltLevel1GTSeed.L1SeedsLogicalExpression = cms.string('32 OR 33 OR 34 OR 35 OR 36 OR 37 OR 38 OR 39 OR 40 OR 41 OR 42 OR 43')
bptx = hltLevel1GTSeed.clone(L1SeedsLogicalExpression = cms.string('0'))
l1Tech4 = hltLevel1GTSeed.clone(L1SeedsLogicalExpression = cms.string('4'))
bscOr = hltLevel1GTSeed.clone(L1SeedsLogicalExpression = cms.string('34'))
bscAnd = hltLevel1GTSeed.clone(L1SeedsLogicalExpression = cms.string('40 OR 41'))
beamHaloVeto = hltLevel1GTSeed.clone(L1SeedsLogicalExpression = cms.string('NOT (36 OR 37 OR 38 OR 39)'))
# BPTX OR
"""
bptxOr = cms.EDFilter("L1Filter",
    inputTag     = cms.InputTag("gtDigis"),
    useAODRecord = cms.bool(False),
    useFinalDecision = cms.bool(False),
    algorithms = cms.vstring("L1_BptxPlusORMinus") # or L1_BptxPlus or L1_BptxMinus
    #algorithms = cms.vstring("L1_BptxPlus")
)
"""

l1Coll = cms.Sequence(bptx+beamHaloVeto)
l1CollBscOr = cms.Sequence(bptx+bscOr+beamHaloVeto)
l1CollBscAnd = cms.Sequence(bptx+bscAnd+beamHaloVeto)
l1NoColl = cms.Sequence(~bptx+beamHaloVeto)
l1NoCollBscOr = cms.Sequence(~bptx+bscOr+beamHaloVeto)
l1NoBPTX = cms.Sequence(beamHaloVeto)
l1NoBPTXBscOr = cms.Sequence(bscOr+beamHaloVeto)

from MinimumBiasAnalysis.MinimumBiasAnalysis.primaryVertexFilter_cfi import *

from MinimumBiasAnalysis.MinimumBiasAnalysis.filterScraping_cfi import *

from CommonTools.RecoAlgos.HBHENoiseFilter_cfi import *

from MinimumBiasAnalysis.MinimumBiasAnalysis.minimumBiasHLTPaths_cfi import *
#hltPhysicsDeclared = minimumBiasHLTFilter.clone(HLTPaths = ['HLT_PhysicsDeclared'])
#hltMinBiasBSCORFilter = minimumBiasHLTFilter.clone(HLTPaths = ['HLT_MinBiasBSC_OR'])
#hltMinBiasPixelSingleTrackFilter = minimumBiasHLTFilter.clone(HLTPaths = ['HLT_MinBiasPixel_SingleTrack'])
hltBscMinBiasORBptxPlusORMinusFilter = minimumBiasHLTFilter.clone(HLTPaths = ['HLT_L1_BscMinBiasOR_BptxPlusORMinus'])

from ExclusiveDijetsAnalysis.ExclusiveDijetsAnalysis.leadingJets_cfi import *
leadingJets.src = "ak5PFJets"

#from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.selectGoodTracks_cfi import *
#selectGoodTracks.cut = "pt > 0.5 & numberOfValidHits > 7 & d0 <= 3.5"
from PhysicsTools.RecoAlgos.recoTrackSelector_cfi import *
recoTrackSelector.src = "generalTracks"
recoTrackSelector.minRapidity = -2.5
recoTrackSelector.maxRapidity = 2.5
recoTrackSelector.ptMin = 0.5
recoTrackSelector.quality = ["highPurity"]
selectGoodTracks = recoTrackSelector
from MinimumBiasAnalysis.MinimumBiasAnalysis.analysisTracks_cfi import *
#analysisTracks.src = "selectGoodTracks"

from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.selectTracksAssociatedToPV_cfi import selectTracksAssociatedToPV as tracksAssociatedToPV
tracksAssociatedToPV.src = "analysisTracks"
tracksAssociatedToPV.MaxDistanceFromVertex = 1.0

from ExclusiveDijetsAnalysis.ExclusiveDijetsAnalysis.tracksOutsideJets_cfi import *
tracksOutsideJets.src = "analysisTracks" 
tracksOutsideJets.JetTag = "leadingJets"
tracksOutsideJets.JetConeSize = 0.5

from ExclusiveDijetsAnalysis.ExclusiveDijetsAnalysis.tracksTransverseRegion_cfi import *
tracksTransverseRegion.src = "analysisTracks"
tracksTransverseRegion.JetTag = "leadingJets"

from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.trackMultiplicity_cfi import * 
trackMultiplicity.TracksTag = "analysisTracks"
trackMultiplicityAssociatedToPV = trackMultiplicity.clone(TracksTag = "tracksAssociatedToPV")
trackMultiplicityOutsideJets = trackMultiplicity.clone(TracksTag = "tracksOutsideJets")
trackMultiplicityTransverseRegion = trackMultiplicity.clone(TracksTag = "tracksTransverseRegion")

from Utilities.AnalysisTools.hcalActivitySummary_cfi import *
hcalActivitySummary.DiscardFlaggedTowers = False
hcalActivitySummaryScale090 = hcalActivitySummary.clone(ApplyEnergyScale = True,EnergyScaleFactorHB = 0.90,EnergyScaleFactorHE = 0.90,EnergyScaleFactorHF = 0.90)
hcalActivitySummaryScale092 = hcalActivitySummary.clone(ApplyEnergyScale = True,EnergyScaleFactorHB = 0.92,EnergyScaleFactorHE = 0.92,EnergyScaleFactorHF = 0.92)
hcalActivitySummaryScale095 = hcalActivitySummary.clone(ApplyEnergyScale = True,EnergyScaleFactorHB = 0.95,EnergyScaleFactorHE = 0.95,EnergyScaleFactorHF = 0.95)
hcalActivitySummaryScale098 = hcalActivitySummary.clone(ApplyEnergyScale = True,EnergyScaleFactorHB = 0.98,EnergyScaleFactorHE = 0.98,EnergyScaleFactorHF = 0.98)
hcalActivitySummaryScale102 = hcalActivitySummary.clone(ApplyEnergyScale = True,EnergyScaleFactorHB = 1.02,EnergyScaleFactorHE = 1.02,EnergyScaleFactorHF = 1.02)
hcalActivitySummaryScale105 = hcalActivitySummary.clone(ApplyEnergyScale = True,EnergyScaleFactorHB = 1.05,EnergyScaleFactorHE = 1.05,EnergyScaleFactorHF = 1.05)
hcalActivitySummaryScale108 = hcalActivitySummary.clone(ApplyEnergyScale = True,EnergyScaleFactorHB = 1.08,EnergyScaleFactorHE = 1.08,EnergyScaleFactorHF = 1.08)
hcalActivitySummaryScale110 = hcalActivitySummary.clone(ApplyEnergyScale = True,EnergyScaleFactorHB = 1.10,EnergyScaleFactorHE = 1.10,EnergyScaleFactorHF = 1.10)

from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.xiTower_cfi import *
from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.xiFromCaloTowers_cfi import *
from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.xiFromJets_cfi import *
xiTower.UseMETInfo = False
xiTower.comEnergy = 7000.0
xiFromCaloTowers.UseMETInfo = False
xiFromCaloTowers.comEnergy = 7000.0
xiFromJets.UseMETInfo = False
xiFromJets.comEnergy = 7000.0

"""
hltMinBiasBSCOR = cms.Sequence(l1CollBscOr)
hltMinBiasBSCAND = cms.Sequence(l1CollBscAnd)
hltMinBiasPixel = cms.Sequence(l1Coll+hltMinBiasPixelSingleTrackFilter)
hltMinBiasBSCORNoColl = cms.Sequence(l1NoCollBscOr)
hltMinBiasPixelNoColl = cms.Sequence(l1NoColl+hltMinBiasPixelSingleTrackFilter)
hltMinBiasBSCORNoBPTX = cms.Sequence(l1NoBPTXBscOr)
hltMinBiasPixelNoBPTX = cms.Sequence(l1NoBPTX+hltMinBiasPixelSingleTrackFilter)
"""
hltBscMinBiasOR = cms.Sequence(l1CollBscOr + hltBscMinBiasORBptxPlusORMinusFilter)
hltBscMinBiasORNoColl = cms.Sequence(l1NoCollBscOr + hltBscMinBiasORBptxPlusORMinusFilter)
hltBscMinBiasORBPTXOR = cms.Sequence(l1NoBPTXBscOr + hltBscMinBiasORBptxPlusORMinusFilter)

#preSelection = cms.Sequence()
offlineSelection = cms.Sequence(primaryVertexFilter+filterScraping+HBHENoiseFilter)
eventSelection = cms.Sequence(offlineSelection)
eventSelectionBPTX = cms.Sequence(bptx+offlineSelection)
eventSelectionL1Tech4 = cms.Sequence(l1Tech4+offlineSelection)
"""
eventSelectionMinBiasBSCOR = cms.Sequence(preSelection+hltMinBiasBSCOR+offlineSelection)
eventSelectionMinBiasBSCAND = cms.Sequence(preSelection+hltMinBiasBSCAND+offlineSelection)
eventSelectionMinBiasPixel = cms.Sequence(preSelection+hltMinBiasPixel+offlineSelection)
eventSelectionMinBiasBSCORNoColl = cms.Sequence(preSelection+hltMinBiasBSCORNoColl+offlineSelection)
eventSelectionMinBiasPixelNoColl = cms.Sequence(preSelection+hltMinBiasPixelNoColl+offlineSelection)
eventSelectionMinBiasBSCORNoBPTX = cms.Sequence(preSelection+hltMinBiasBSCORNoBPTX+offlineSelection)
eventSelectionMinBiasPixelNoBPTX = cms.Sequence(preSelection+hltMinBiasPixelNoBPTX+offlineSelection)
"""
eventSelectionBscMinBiasOR = cms.Sequence(hltBscMinBiasOR+offlineSelection)
eventSelectionBscMinBiasORNoColl = cms.Sequence(hltBscMinBiasORNoColl+offlineSelection)
eventSelectionBscMinBiasORBPTXOR = cms.Sequence(hltBscMinBiasORBPTXOR+offlineSelection) 

from Utilities.AnalysisTools.hcalActivityFilter_cfi import hcalActivityFilter
hcalActivityFilter.EnergyThresholdHB = 1.25
hcalActivityFilter.EnergyThresholdHE = 1.9
hcalActivityFilter.EnergyThresholdHF = 4.0
#hcalActivityFilter.EnergyThresholdEB = 0.6
#hcalActivityFilter.EnergyThresholdEE = 2.45
hcalVetoHFPlus = hcalActivityFilter.clone(NTowersMaxHFPlus = 0)
hcalVetoHFMinus = hcalActivityFilter.clone(NTowersMaxHFMinus = 0)
hcalVetoHEHFPlus = hcalActivityFilter.clone(NTowersMaxHEPlus = 0, NTowersMaxHFPlus = 0)
hcalVetoHEHFMinus = hcalActivityFilter.clone(NTowersMaxHEMinus = 0, NTowersMaxHFMinus = 0)
hcalVetoHFPlusAndMinus = hcalActivityFilter.clone(NTowersMaxHFPlus = 0, NTowersMaxHFMinus = 0)
hcalVetoHEHFPlusAndMinus = hcalActivityFilter.clone(NTowersMaxHEPlus = 0, NTowersMaxHFPlus = 0, NTowersMaxHEMinus = 0, NTowersMaxHFMinus = 0)
hcalVetoSumEMaxHFPlus4 = hcalActivityFilter.clone(SumEMaxHFPlus = 4.0)
hcalVetoSumEMaxHFPlus8 = hcalActivityFilter.clone(SumEMaxHFPlus = 8.0)
hcalVetoSumEMaxHFPlus12 = hcalActivityFilter.clone(SumEMaxHFPlus = 12.0)
hcalVetoSumEMaxHFPlus16 = hcalActivityFilter.clone(SumEMaxHFPlus = 16.0)
hcalVetoSumEMaxHFMinus4 = hcalActivityFilter.clone(SumEMaxHFMinus = 4.0)
hcalVetoSumEMaxHFMinus8 = hcalActivityFilter.clone(SumEMaxHFMinus = 8.0)
hcalVetoSumEMaxHFMinus12 = hcalActivityFilter.clone(SumEMaxHFMinus = 12.0)
hcalVetoSumEMaxHFMinus16 = hcalActivityFilter.clone(SumEMaxHFMinus = 16.0)

eventSelectionBscMinBiasORHFVetoPlus = cms.Sequence(eventSelectionBscMinBiasOR+hcalVetoHFPlus)
eventSelectionBscMinBiasORHFVetoMinus = cms.Sequence(eventSelectionBscMinBiasOR+hcalVetoHFMinus)
eventSelectionBscMinBiasORHEHFVetoPlus = cms.Sequence(eventSelectionBscMinBiasOR+hcalVetoHEHFPlus)
eventSelectionBscMinBiasORHEHFVetoMinus = cms.Sequence(eventSelectionBscMinBiasOR+hcalVetoHEHFMinus)
eventSelectionBscMinBiasORSumEMaxHFPlus4 = cms.Sequence(eventSelectionBscMinBiasOR+hcalVetoSumEMaxHFPlus4)
eventSelectionBscMinBiasORSumEMaxHFPlus8 = cms.Sequence(eventSelectionBscMinBiasOR+hcalVetoSumEMaxHFPlus8)
eventSelectionBscMinBiasORSumEMaxHFPlus12 = cms.Sequence(eventSelectionBscMinBiasOR+hcalVetoSumEMaxHFPlus12)
eventSelectionBscMinBiasORSumEMaxHFPlus16 = cms.Sequence(eventSelectionBscMinBiasOR+hcalVetoSumEMaxHFPlus16)
eventSelectionBscMinBiasORSumEMaxHFMinus4 = cms.Sequence(eventSelectionBscMinBiasOR+hcalVetoSumEMaxHFMinus4)
eventSelectionBscMinBiasORSumEMaxHFMinus8 = cms.Sequence(eventSelectionBscMinBiasOR+hcalVetoSumEMaxHFMinus8)
eventSelectionBscMinBiasORSumEMaxHFMinus12 = cms.Sequence(eventSelectionBscMinBiasOR+hcalVetoSumEMaxHFMinus12)
eventSelectionBscMinBiasORSumEMaxHFMinus16 = cms.Sequence(eventSelectionBscMinBiasOR+hcalVetoSumEMaxHFMinus16)

#jets = cms.Sequence(leadingJets)
#tracks = cms.Sequence(selectGoodTracks*
#                      tracksAssociatedToPV*
#                      tracksOutsideJets+
#                      tracksTransverseRegion) 
#tracks = cms.Sequence(selectGoodTracks*analysisTracks)
tracks = cms.Sequence(analysisTracks)
edmDump = cms.Sequence(trackMultiplicity+
                       hcalActivitySummary+hcalActivitySummaryScale090+hcalActivitySummaryScale092+
                       hcalActivitySummaryScale095+hcalActivitySummaryScale098+
                       hcalActivitySummaryScale102+hcalActivitySummaryScale105+
                       hcalActivitySummaryScale108+hcalActivitySummaryScale110+
                       xiTower+xiFromCaloTowers+xiFromJets)
