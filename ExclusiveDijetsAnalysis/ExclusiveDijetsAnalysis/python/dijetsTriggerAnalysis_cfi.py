import FWCore.ParameterSet.Config as cms

dijetsTriggerAnalysis = cms.EDAnalyzer("DijetsTriggerAnalyzer",
  gtDigisTag = cms.InputTag("gtDigis"),
  gctDigisTag = cms.InputTag("gctDigis"), 
  l1GTObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
  # ET sum/tower count per HF ring (HFRingEtSums, HFBitCounts)
  hfRingSumType = cms.string("HFBitCounts"),
  hfRingThreshold = cms.uint32(5),
  l1TriggerNames = cms.vstring('L1_SingleJet36_FwdVeto','L1_SingleJet36','L1_DoubleJet36_Central','L1_SingleEG18','L1_DoubleEG5') 
)
