import FWCore.ParameterSet.Config as cms

from Utilities.AnalysisSequences.minimumBiasAnalysisSequences_cff import *

# Add EtaMax
# Change thresholds here if needed
from ForwardAnalysis.Utilities.pfCandidateSelector_cfi import pfCandidateSelector as pfCandidateNoiseThresholds
from ForwardAnalysis.Utilities.ExcludeHFEdgesStringCut import ExcludeHFEdgesStringCut
from ForwardAnalysis.Utilities.PFCandidateNoiseStringCut import PFCandidateNoiseStringCut
#from ForwardAnalysis.Utilities.pfThresholds_cfi import pfThresholds
#from MinimumBiasAnalysis.MinimumBiasAnalysis.pfThresholds_cfi import pfThresholds
from MinimumBiasAnalysis.MinimumBiasAnalysis.pfThresholds_new_cfi import pfThresholds

#pfCandidateNoiseThresholds.cut = PFCandidateNoiseStringCut(pfThresholds).cut()
pfStrCutHFEdges = ExcludeHFEdgesStringCut().cut()
pfStrCutNoise = PFCandidateNoiseStringCut(pfThresholds).cut()
pfStrCut = '%s & %s' % (pfStrCutHFEdges,pfStrCutNoise)
pfCandidateNoiseThresholds.cut = pfStrCut
pfCandidateHFEdges = pfCandidateNoiseThresholds.clone( cut = pfStrCutHFEdges )

from ForwardAnalysis.Utilities.etaMaxCandViewSelector_cfi import etaMaxCandViewSelector as etaMaxPFCands
from ForwardAnalysis.Utilities.etaMinCandViewSelector_cfi import etaMinCandViewSelector as etaMinPFCands
etaMaxPFCands.src = "pfCandidateNoiseThresholds"
etaMinPFCands.src = "pfCandidateNoiseThresholds"

from Utilities.AnalysisSequences.genChargedParticles_cfi import genChargedParticles
from Utilities.AnalysisSequences.genStableParticles_cfi import genStableParticles
genStableParticles.cut = 'status = 1 & ( ( pdgId != 2212 ) | ( pdgId == 2212 & abs(pz) < %f ) )' % (0.75*3500.0)
genProtonDissociative = genStableParticles.clone( cut = 'pdgId == 9902210' )
etaMaxGen = etaMaxPFCands.clone(src = "genStableParticles")
etaMinGen = etaMinPFCands.clone(src = "genStableParticles")

from Utilities.AnalysisSequences.edmNtupleCandView_cfi import edmNtupleCandView
edmNtupleEtaMax = edmNtupleCandView.clone(src = "etaMaxPFCands")
edmNtupleEtaMax.variables = cms.VPSet( cms.PSet( tag = cms.untracked.string("etaMax"),
                                                 quantity = cms.untracked.string("eta") ) )
edmNtupleEtaMin = edmNtupleCandView.clone(src = "etaMinPFCands")
edmNtupleEtaMin.variables = cms.VPSet( cms.PSet( tag = cms.untracked.string("etaMin"),
                                                 quantity = cms.untracked.string("eta") ) )

edmNtupleMxGen = edmNtupleCandView.clone(src = "genProtonDissociative")
edmNtupleMxGen.variables = cms.VPSet( cms.PSet( tag = cms.untracked.string("Mx"),
                                                quantity = cms.untracked.string("mass") ) )
edmNtupleEtaMaxGen = edmNtupleEtaMax.clone(src = "etaMaxGen")
edmNtupleEtaMinGen = edmNtupleEtaMin.clone(src = "etaMinGen")

from Utilities.AnalysisSequences.etaMaxSelector_cfi import etaMaxSelector as etaMaxFilter
etaMaxFilter.src = "etaMaxPFCands"
from Utilities.AnalysisSequences.etaMinSelector_cfi import etaMinSelector as etaMinFilter
etaMinFilter.src = "etaMinPFCands"

etaMaxGenFilter = etaMaxFilter.clone(src = "etaMaxGen")
etaMinGenFilter = etaMinFilter.clone(src = "etaMinGen")

############
from ForwardAnalysis.Utilities.castorActivityFilter_cfi import castorActivityFilter
castorActivityFilter.CastorRecHitTag = "castorRecHitCorrector"
castorActivityFilter.SumEMaxCastor = 250.

castorInvalidDataFilter = cms.EDFilter("CastorInvalidDataFilter")
castorVeto = cms.Sequence(castorInvalidDataFilter + castorActivityFilter)
castorTag = cms.Sequence(castorInvalidDataFilter + ~castorActivityFilter) 
############
primaryVertexFilterLooseNDOF2 = primaryVertexFilter.clone()
primaryVertexFilterLooseNDOF2.cut = cms.string("!isFake && ndof > 2 && abs(z) <= 15 && position.Rho <= 2")
primaryVertexFilterLooseNDOF0 = primaryVertexFilter.clone()
primaryVertexFilterLooseNDOF0.cut = cms.string("!isFake && ndof > 0 && abs(z) <= 15 && position.Rho <= 2")

offlineSelectionLooseNDOF0 = cms.Sequence(primaryVertexFilterLooseNDOF0+filterScraping+HBHENoiseFilter)
offlineSelectionNoVertex = cms.Sequence(filterScraping+HBHENoiseFilter)

eventSelectionBscMinBiasORLooseNDOF0 = cms.Sequence(hltBscMinBiasOR+offlineSelectionLooseNDOF0)
eventSelectionBscMinBiasORNoVertex = cms.Sequence(hltBscMinBiasOR+offlineSelectionNoVertex)
eventSelectionBscMinBiasORNoCollLooseNDOF0 = cms.Sequence(hltBscMinBiasORNoColl+offlineSelectionLooseNDOF0)
eventSelectionBscMinBiasORNoCollNoVertex = cms.Sequence(hltBscMinBiasORNoColl+offlineSelectionNoVertex)

eventSelectionBscMinBiasOREtaMaxFilter = cms.Sequence(eventSelectionBscMinBiasOR+etaMaxFilter)
eventSelectionBscMinBiasORLooseNDOF0EtaMaxFilter = cms.Sequence(eventSelectionBscMinBiasORLooseNDOF0+etaMaxFilter)
eventSelectionBscMinBiasOREtaMinFilter = cms.Sequence(eventSelectionBscMinBiasOR+etaMinFilter)
eventSelectionBscMinBiasORLooseNDOF0EtaMinFilter = cms.Sequence(eventSelectionBscMinBiasORLooseNDOF0+etaMinFilter)

eventSelectionBscMinBiasOREtaMinFilterCastorVeto = cms.Sequence(eventSelectionBscMinBiasOR+
                                                                etaMinFilter+castorVeto)
eventSelectionBscMinBiasORLooseNDOF0EtaMinFilterCastorVeto = cms.Sequence(eventSelectionBscMinBiasORLooseNDOF0+
                                                                          etaMinFilter+castorVeto)
eventSelectionBscMinBiasOREtaMinFilterCastorTag = cms.Sequence(eventSelectionBscMinBiasOR+
                                                               etaMinFilter+castorTag)
eventSelectionBscMinBiasORLooseNDOF0EtaMinFilterCastorTag = cms.Sequence(eventSelectionBscMinBiasORLooseNDOF0+
                                                                         etaMinFilter+castorTag)

vertices = cms.Sequence(analysisVertices)
tracks = cms.Sequence(analysisTracks)
pfCandidates = cms.Sequence(pfCandidateNoiseThresholds*etaMaxPFCands+etaMinPFCands)
edmDump = cms.Sequence(hcalActivitySummary+hcalActivitySummaryScale090+hcalActivitySummaryScale092+
                       hcalActivitySummaryScale095+hcalActivitySummaryScale098+
                       hcalActivitySummaryScale102+hcalActivitySummaryScale105+
                       hcalActivitySummaryScale108+hcalActivitySummaryScale110+
                       edmNtupleEtaMax+edmNtupleEtaMin)
