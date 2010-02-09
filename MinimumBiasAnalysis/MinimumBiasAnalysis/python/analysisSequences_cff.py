import FWCore.ParameterSet.Config as cms

lumiSectionSel = cms.EDFilter("LumiSectionSelection",
      Runs = cms.vuint32(124023),
      LumiSectionMin = cms.vuint32(41),
      LumiSectionMax = cms.vuint32(96), 
      applyfilter = cms.untracked.bool(True)
)

from L1TriggerConfig.L1GtConfigProducers.L1GtTriggerMaskTechTrigConfig_cff import *
from HLTrigger.HLTfilters.hltLevel1GTSeed_cfi import *
hltLevel1GTSeed.L1TechTriggerSeeding = cms.bool(True)
#hltLevel1GTSeed.L1SeedsLogicalExpression = cms.string('32 OR 33 OR 34 OR 35 OR 36 OR 37 OR 38 OR 39 OR 40 OR 41 OR 42 OR 43')
# BPTX and BSC(OR)
#hltLevel1GTSeed.L1SeedsLogicalExpression = cms.string('0 AND 34')
#hltLevel1GTSeed.L1SeedsLogicalExpression = cms.string('0 AND 34 AND (NOT 36) AND (NOT 37) AND (NOT 38) AND (NOT 39)')
bptx = hltLevel1GTSeed.clone(L1SeedsLogicalExpression = cms.string('0'))
bscOr = hltLevel1GTSeed.clone(L1SeedsLogicalExpression = cms.string('34'))
beamHaloVeto = hltLevel1GTSeed.clone(L1SeedsLogicalExpression = cms.string('NOT (36 OR 37 OR 38 OR 39)'))
#l1tech = cms.Sequence(hltLevel1GTSeed)
#l1tech = cms.Sequence(bptx+bscOr+beamHaloVeto)
# BPTX OR
bptxOr = cms.EDFilter("L1Filter",
    inputTag     = cms.InputTag("gtDigis"),
    useAODRecord = cms.bool(False),
    useFinalDecision = cms.bool(False),
    algorithms = cms.vstring("L1_BptxPlusORMinus") # or L1_BptxPlus or L1_BptxMinus
    #algorithms = cms.vstring("L1_BptxPlus")
)
"""
bptxOr = hltLevel1GTSeed.clone(
    #L1TechTriggerSeeding = False,
    L1SeedsLogicalExpression = cms.string('82')
)
"""
l1Coll = cms.Sequence(bptx+beamHaloVeto)
l1CollBscOr = cms.Sequence(bptx+bscOr+beamHaloVeto)
l1NoCollBscOr = cms.Sequence(~bptx+bscOr+beamHaloVeto)
l1NoBPTXBscOr = cms.Sequence(bscOr+beamHaloVeto)


primaryVertexFilter = cms.EDFilter("GoodVertexFilter",
    vertexCollection = cms.InputTag('offlinePrimaryVertices'),
    #minimumNumberOfTracks = cms.uint32(3),
    minimumNumberOfTracks = cms.uint32(2),
    maxAbsZ = cms.double(15),	
    maxd0 = cms.double(2)	
)

filterScraping = cms.EDFilter("FilterOutScraping",
    applyfilter = cms.untracked.bool(True),
    debugOn = cms.untracked.bool(True),
    numtrack = cms.untracked.uint32(10),
    thresh = cms.untracked.double(0.2)
)

from MinimumBiasAnalysis.MinimumBiasAnalysis.minimumBiasHLTPaths_cfi import *
hltMinBiasBSCORFilter = minimumBiasHLTFilter.clone(HLTPaths = ['HLT_MinBiasBSC_OR'])
hltMinBiasPixelSingleTrackFilter = minimumBiasHLTFilter.clone(HLTPaths = ['HLT_MinBiasPixel_SingleTrack'])

from ExclusiveDijetsAnalysis.ExclusiveDijetsAnalysis.leadingJets_cfi import *
#leadingJets.src = "sisCone7PFJets"
leadingJets.src = "sisCone7CaloJets"

#from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.selectGoodTracks_cfi import *
#selectGoodTracks.cut = "pt > 0.5 & numberOfValidHits > 7 & d0 <= 3.5"

from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.selectTracksAssociatedToPV_cfi import *
#selectTracksAssociatedToPV.src = "selectGoodTracks"
#selectTracksAssociatedToPV.src = cms.InputTag("generalTracks","","RETRACK")
#selectTracksAssociatedToPV.VertexTag = cms.InputTag("offlinePrimaryVertices","","REVERTEX")
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
#trackMultiplicity.TracksTag = cms.InputTag("generalTracks","","RETRACK")
trackMultiplicityAssociatedToPV = trackMultiplicity.clone(TracksTag = "selectTracksAssociatedToPV")
trackMultiplicityOutsideJets = trackMultiplicity.clone(TracksTag = "tracksOutsideJets")
trackMultiplicityTransverseRegion = trackMultiplicity.clone(TracksTag = "tracksTransverseRegion")

from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.hfTower_cfi import *
from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.xiTower_cfi import *
from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.xiFromCaloTowers_cfi import *
from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.xiFromJets_cfi import *
xiTower.UseMETInfo = False
xiTower.comEnergy = 900.0
xiFromCaloTowers.UseMETInfo = False
xiFromCaloTowers.comEnergy = 900.0
xiFromJets.UseMETInfo = False
xiFromJets.comEnergy = 900.0

#hltBscOr = cms.Sequence(l1CollBscOr)
#hltNoColl = cms.Sequence(l1NoColl)
#hltNoBPTX = cms.Sequence(l1NoBPTX)
hltMinBiasBSCOR = cms.Sequence(l1CollBscOr+hltMinBiasBSCORFilter)
hltMinBiasPixel = cms.Sequence(l1Coll+hltMinBiasPixelSingleTrackFilter)

eventSelection = cms.Sequence(l1Coll+primaryVertexFilter+filterScraping)
#eventSelectionNoColl = cms.Sequence(hltNoColl+primaryVertexFilter+filterScraping)
#eventSelectionNoBPTX = cms.Sequence(hltNoBPTX+primaryVertexFilter+filterScraping)
eventSelectionMinBiasBSCOR = cms.Sequence(hltMinBiasBSCOR+primaryVertexFilter+filterScraping)
eventSelectionMinBiasPixel = cms.Sequence(hltMinBiasPixel+primaryVertexFilter+filterScraping)

#jets = cms.Sequence(L2L3CorJetSC5PF+L2L3CorJetSC7PF*leadingJets)
#tracks = cms.Sequence(selectGoodTracks*
#                      selectTracksAssociatedToPV*
#                      tracksOutsideJets+
#                      tracksTransverseRegion) 
tracks = cms.Sequence(selectTracksAssociatedToPV)
#edmDump = cms.Sequence(trackMultiplicity+
#                       trackMultiplicityAssociatedToPV+
#                       trackMultiplicityOutsideJets+
#                       trackMultiplicityTransverseRegion+
#                       hfTower+
#                       xiTower)
edmDump = cms.Sequence(trackMultiplicity+
                       trackMultiplicityAssociatedToPV+
                       hfTower+
                       xiTower+xiFromCaloTowers+xiFromJets)
