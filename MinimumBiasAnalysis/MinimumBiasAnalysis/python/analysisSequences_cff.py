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

from MinimumBiasAnalysis.MinimumBiasAnalysis.minimumBiasHLTPaths_cfi import *
#hltPhysicsDeclared = minimumBiasHLTFilter.clone(HLTPaths = ['HLT_PhysicsDeclared'])
#hltMinBiasBSCORFilter = minimumBiasHLTFilter.clone(HLTPaths = ['HLT_MinBiasBSC_OR'])
#hltMinBiasPixelSingleTrackFilter = minimumBiasHLTFilter.clone(HLTPaths = ['HLT_MinBiasPixel_SingleTrack'])
hltBscMinBiasORBptxPlusORMinusFilter = minimumBiasHLTFilter.clone(HLTPaths = ['HLT_L1_BscMinBiasOR_BptxPlusORMinus'])

from ExclusiveDijetsAnalysis.ExclusiveDijetsAnalysis.leadingJets_cfi import *
leadingJets.src = "ak7PFJets"

#from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.selectGoodTracks_cfi import *
#selectGoodTracks.cut = "pt > 0.5 & numberOfValidHits > 7 & d0 <= 3.5"
from PhysicsTools.RecoAlgos.recoTrackSelector_cfi import *
recoTrackSelector.src = "generalTracks"
recoTrackSelector.minRapidity = -2.0
recoTrackSelector.maxRapidity = 2.0
recoTrackSelector.ptMin = 0.5
recoTrackSelector.quality = ["highPurity"]
selectGoodTracks = recoTrackSelector

from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.selectTracksAssociatedToPV_cfi import *
selectTracksAssociatedToPV.src = "selectGoodTracks"
selectTracksAssociatedToPV.MaxDistanceFromVertex = 1.0

from ExclusiveDijetsAnalysis.ExclusiveDijetsAnalysis.tracksOutsideJets_cfi import *
tracksOutsideJets.src = "selectTracksAssociatedToPV" 
tracksOutsideJets.JetTag = "leadingJets"
tracksOutsideJets.JetConeSize = 0.7

from ExclusiveDijetsAnalysis.ExclusiveDijetsAnalysis.tracksTransverseRegion_cfi import *
tracksTransverseRegion.src = "selectTracksAssociatedToPV"
tracksTransverseRegion.JetTag = "leadingJets"

from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.trackMultiplicity_cfi import * 
#trackMultiplicity.TracksTag = "selectGoodTracks"
trackMultiplicityAssociatedToPV = trackMultiplicity.clone(TracksTag = "selectTracksAssociatedToPV")
trackMultiplicityOutsideJets = trackMultiplicity.clone(TracksTag = "tracksOutsideJets")
trackMultiplicityTransverseRegion = trackMultiplicity.clone(TracksTag = "tracksTransverseRegion")

from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.hfTower_cfi import *
hfTower.DiscardFlaggedTowers = True
hfTowerScale090 = hfTower.clone(ApplyEnergyScale = True,EnergyScaleFactor = 0.90)
hfTowerScale092 = hfTower.clone(ApplyEnergyScale = True,EnergyScaleFactor = 0.92)
hfTowerScale095 = hfTower.clone(ApplyEnergyScale = True,EnergyScaleFactor = 0.95)
hfTowerScale098 = hfTower.clone(ApplyEnergyScale = True,EnergyScaleFactor = 0.98)
hfTowerScale102 = hfTower.clone(ApplyEnergyScale = True,EnergyScaleFactor = 1.02)
hfTowerScale105 = hfTower.clone(ApplyEnergyScale = True,EnergyScaleFactor = 1.05)
hfTowerScale108 = hfTower.clone(ApplyEnergyScale = True,EnergyScaleFactor = 1.08)
hfTowerScale110 = hfTower.clone(ApplyEnergyScale = True,EnergyScaleFactor = 1.10)

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
hltBscMinBiasOR = cms.Sequence(l1Coll+hltBscMinBiasORBptxPlusORMinusFilter)
hltBscMinBiasORNoColl = cms.Sequence(l1NoColl+hltBscMinBiasORBptxPlusORMinusFilter)
hltBscMinBiasORBPTXOR = cms.Sequence(l1NoBPTX+hltBscMinBiasORBptxPlusORMinusFilter)

#preSelection = cms.Sequence()
offlineSelection = cms.Sequence(primaryVertexFilter+filterScraping)
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

#jets = cms.Sequence(leadingJets)
#tracks = cms.Sequence(selectGoodTracks*
#                      selectTracksAssociatedToPV*
#                      tracksOutsideJets+
#                      tracksTransverseRegion) 
tracks = cms.Sequence(selectGoodTracks*selectTracksAssociatedToPV)
#edmDump = cms.Sequence(trackMultiplicity+
#                       trackMultiplicityAssociatedToPV+
#                       trackMultiplicityOutsideJets+
#                       trackMultiplicityTransverseRegion+
#                       hfTower+
#                       xiTower)
edmDump = cms.Sequence(trackMultiplicity+
                       trackMultiplicityAssociatedToPV+
                       hfTower+hfTowerScale090+hfTowerScale092+hfTowerScale095+hfTowerScale098+
                       hfTowerScale102+hfTowerScale105+hfTowerScale108+hfTowerScale110+
                       xiTower+xiFromCaloTowers+xiFromJets)
