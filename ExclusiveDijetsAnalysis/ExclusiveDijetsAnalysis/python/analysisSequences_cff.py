import FWCore.ParameterSet.Config as cms

#l1filter = cms.EDFilter("L1TriggerTestFilter",
#  HFRingETSumThreshold = cms.int32(0),
#  L1TriggerNames = cms.vstring("L1_SingleJet30"),
#  AccessL1GctHFRingEtSums = cms.untracked.bool(True) 
#)

from ExclusiveDijetsAnalysis.ExclusiveDijetsAnalysis.exclusiveDijetsHLTPaths_cfi import *

#from JetMETCorrections.Configuration.L2L3Corrections_Summer08Redigi_cff import *

from ExclusiveDijetsAnalysis.ExclusiveDijetsAnalysis.leadingJets_cfi import *
#leadingJets.src = "L2L3CorJetSC7PF"
leadingJets.src = "ak7PFJets"

from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.selectGoodTracks_cfi import *
selectGoodTracks.cut = "pt > 0.5 & numberOfValidHits > 7 & d0 <= 3.5"

from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.selectTracksAssociatedToPV_cfi import *
selectTracksAssociatedToPV.src = "selectGoodTracks"
selectTracksAssociatedToPV.MaxDistanceFromVertex = 0.3

from ExclusiveDijetsAnalysis.ExclusiveDijetsAnalysis.tracksOutsideJets_cfi import *
tracksOutsideJets.src = "selectTracksAssociatedToPV" 
tracksOutsideJets.JetTag = "leadingJets"
tracksOutsideJets.JetConeSize = 0.7

from ExclusiveDijetsAnalysis.ExclusiveDijetsAnalysis.tracksTransverseRegion_cfi import *
tracksTransverseRegion.src = "selectTracksAssociatedToPV"
tracksTransverseRegion.JetTag = "leadingJets"

from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.trackMultiplicity_cfi import * 
trackMultiplicity.TracksTag = "selectGoodTracks"
trackMultiplicityAssociatedToPV = trackMultiplicity.clone(TracksTag = "selectTracksAssociatedToPV")
trackMultiplicityOutsideJets = trackMultiplicity.clone(TracksTag = "tracksOutsideJets")
trackMultiplicityTransverseRegion = trackMultiplicity.clone(TracksTag = "tracksTransverseRegion")

from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.hfTower_cfi import *
from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.xiTower_cfi import *
from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.xiFromCaloTowers_cfi import * 
xiTower.UseMETInfo = False
xiFromCaloTowers.ParticlesTag = cms.InputTag("towerMaker")
xiFromCaloTowers.UseMETInfo = False

#from ExclusiveDijetsAnalysis.ExclusiveDijetsAnalysis.BTagging_cff import *

analysisBeforeSelection = cms.EDAnalyzer("SimpleDijetsAnalyzer",
    JetTag = cms.InputTag("iterativeCone5CaloJets")
)
analysisAfterSelection = analysisBeforeSelection.clone()

hlt = cms.Sequence(exclusiveDijetsHLTFilter)
jets = cms.Sequence(leadingJets)
tracks = cms.Sequence(selectGoodTracks*
                      selectTracksAssociatedToPV*
                      tracksOutsideJets+
                      tracksTransverseRegion) 
edmDump = cms.Sequence(trackMultiplicity+
                       trackMultiplicityAssociatedToPV+
                       trackMultiplicityOutsideJets+
                       trackMultiplicityTransverseRegion+
                       hfTower+
                       xiTower+xiFromCaloTowers)
analysisBefore = cms.Sequence(analysisBeforeSelection)
analysisAfter = cms.Sequence(hlt*analysisAfterSelection)
