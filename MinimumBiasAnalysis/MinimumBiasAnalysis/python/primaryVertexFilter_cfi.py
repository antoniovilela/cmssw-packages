import FWCore.ParameterSet.Config as cms

primaryVertexFilter = cms.EDFilter("GoodVertexFilter",
    vertexCollection = cms.InputTag('offlinePrimaryVertices'),
    minimumNDOF = cms.uint32(2),
    maxAbsZ = cms.double(15.0),	
    maxd0 = cms.double(2.0)
)
