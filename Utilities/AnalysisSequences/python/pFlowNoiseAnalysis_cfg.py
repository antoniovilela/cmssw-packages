import FWCore.ParameterSet.Config as cms

process = cms.Process("Analysis")

process.load("FWCore.MessageLogger.MessageLogger_cfi")

process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring()
)

process.TFileService = cms.Service("TFileService",
    fileName = cms.string('analysisHistos_pFlowNoiseAnalysis.root')
)

#############
process.load('Utilities.AnalysisTools.pFlowNoiseAnalyzer_cfi')
process.load('Utilities.AnalysisSequences.hltFilter_cfi')
process.hltBPTXPlusOrMinusOnly = process.hltFilter.clone(
    HLTPaths = ['HLT_L1_BPTX_PlusOnly','HLT_L1_BPTX_MinusOnly']
)
#############
process.generalTracksFilter = cms.EDFilter("TrackCountFilter",
    src = cms.InputTag('generalTracks'),
    minNumber = cms.uint32(1) 
)
#process.pixelLessTracksFilter = cms.EDFilter("TrackCountFilter",
#    src = cms.InputTag('ctfPixelLess'),
#    minNumber = cms.uint32(1) 
#)
process.pixelTracksFilter = cms.EDFilter("TrackCountFilter",
    src = cms.InputTag('pixelTracks'),
    minNumber = cms.uint32(1) 
)
process.vertexVeto = cms.Sequence(~process.primaryVertexFilter)
process.trackVeto = cms.Sequence(~process.generalTracksFilter + ~process.pixelTracksFilter) 
#############

process.pFlowAnalysisNoSel = process.pFlowAnalysis.clone()
process.pFlowAnalysisColl = process.pFlowAnalysis.clone()
process.pFlowAnalysisNoColl = process.pFlowAnalysis.clone()
process.pFlowAnalysisNoCollNoVtx = process.pFlowAnalysis.clone()
process.pFlowAnalysisNoCollNoTrk = process.pFlowAnalysis.clone()
process.pFlowAnalysisBPTXPlusOrMinusOnly = process.pFlowAnalysis.clone()
process.pFlowAnalysisBPTXPlusOrMinusOnlyNoVtx = process.pFlowAnalysis.clone()
process.pFlowAnalysisBPTXPlusOrMinusOnlyNoTrk = process.pFlowAnalysis.clone()

process.analysisNoSel = cms.Path(process.pFlowAnalysisNoSel)
process.analysisColl = cms.Path(process.l1CollBscOr+
                                process.offlineSelection+
                                process.pFlowAnalysisColl)
process.analysisNoColl = cms.Path(process.l1NoColl+
                                  process.pFlowAnalysisNoColl)
process.analysisNoCollNoVtx = cms.Path(process.l1NoColl+
                                       process.vertexVeto+ 
                                       process.pFlowAnalysisNoCollNoVtx)
process.analysisNoCollNoTrk = cms.Path(process.l1NoColl+
                                       process.trackVeto+
                                       process.pFlowAnalysisNoCollNoTrk)
process.analysisBPTXPlusOrMinusOnly = cms.Path(process.hltBPTXPlusOrMinusOnly+
                                               process.pFlowAnalysisBPTXPlusOrMinusOnly) 
process.analysisBPTXPlusOrMinusOnlyNoVtx = cms.Path(process.hltBPTXPlusOrMinusOnly+
                                                    process.vertexVeto+
                                                    process.pFlowAnalysisBPTXPlusOrMinusOnly)
process.analysisBPTXPlusOrMinusOnlyNoTrk = cms.Path(process.hltBPTXPlusOrMinusOnly+
                                                    process.trackVeto+
                                                    process.pFlowAnalysisBPTXPlusOrMinusOnly)

