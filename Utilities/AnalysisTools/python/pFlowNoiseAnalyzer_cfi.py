import FWCore.ParameterSet.Config as cms

pFlowNoiseAnalyzer = cms.EDAnalyzer("PFlowNoiseAnalyzer",
    particleFlowTag = cms.InputTag("particleFlow")
)
