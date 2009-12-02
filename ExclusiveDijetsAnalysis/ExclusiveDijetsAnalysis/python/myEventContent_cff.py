import FWCore.ParameterSet.Config as cms

PatEventContent = cms.PSet(
    outputCommands = cms.untracked.vstring(
          'keep *_selectedLayer1Jets*_*_*',
          'keep *_layer1METs_*_*'
    )
)

MyEventContent_tracks = cms.PSet(
    outputCommands = cms.untracked.vstring(
          'keep *_selectTracksAssociatedToPV_*_Analysis'
    )
)

MyEventContent_tracks_reduced = cms.PSet(
    outputCommands = cms.untracked.vstring()
)

MyEventContent_tracks_expanded = cms.PSet(
    outputCommands = cms.untracked.vstring(
          'keep *_selectGoodTracks_*_Analysis',
          'keep *_selectTracksAssociatedToPV_*_Analysis',
          'keep *_tracksOutsideJets_*_Analysis',
          'keep *_tracksTransverseRegion_*_Analysis' 
    )
)

MyEventContent_jets = cms.PSet(
    outputCommands = cms.untracked.vstring(
          'keep *_sisCone5GenJets_*_*',
          'keep *_sisCone7GenJets_*_*',
          'keep *_sisCone5PFJets_*_*',
          'keep *_sisCone7PFJets_*_*',
          'keep *_L2L3CorJetSC5PF_*_Analysis',
          'keep *_L2L3CorJetSC7PF_*_Analysis'
    )
)

MyEventContent_jets_reduced = cms.PSet(
    outputCommands = cms.untracked.vstring(
          'keep *_sisCone7GenJets_*_*',
          'keep *_sisCone7PFJets_*_*'
    )
)

MyEventContent_jets_expanded = cms.PSet(
    outputCommands = cms.untracked.vstring(
          'keep *_kt4GenJets_*_*',
          'keep *_kt6GenJets_*_*',
          'keep *_sisCone5GenJets_*_*',
          'keep *_sisCone7GenJets_*_*', 
          'keep *_kt4CaloJets_*_*',
          'keep *_kt6CaloJets_*_*',
          'keep *_sisCone5CaloJets_*_*',
          'keep *_sisCone7CaloJets_*_*',
          'keep *_kt4PFJets_*_*',
          'keep *_kt6PFJets_*_*',
          'keep *_sisCone5PFJets_*_*',
          'keep *_sisCone7PFJets_*_*',
          'keep *_L2L3CorJetKT4Calo_*_Analysis',
          'keep *_L2L3CorJetKT6Calo_*_Analysis',
          'keep *_L2L3CorJetSC5Calo_*_Analysis',
          'keep *_L2L3CorJetSC7Calo_*_Analysis',
          'keep *_L2L3CorJetKT4PF_*_Analysis',
          'keep *_L2L3CorJetKT6PF_*_Analysis',
          'keep *_L2L3CorJetSC5PF_*_Analysis',
          'keep *_L2L3CorJetSC7PF_*_Analysis'
    )
)

MyEventContent_btag = cms.PSet(
    outputCommands = cms.untracked.vstring(
          'keep *_jetBProbabilityBJetTagsSC5PF_*_*',
          'keep *_jetProbabilityBJetTagsSC5PF_*_*',
          'keep *_jetBProbabilityBJetTagsSC7PF_*_*',
          'keep *_jetProbabilityBJetTagsSC7PF_*_*'
    )
)

MyEventContent_btag_reduced = cms.PSet(
    outputCommands = cms.untracked.vstring(
          'keep *_jetBProbabilityBJetTagsSC7PF_*_*',
          'keep *_jetProbabilityBJetTagsSC7PF_*_*'
    )
)

MyEventContent_btag_expanded = cms.PSet(
    outputCommands = cms.untracked.vstring(
          'keep *_trackCountingHighEffBJetTagsSC5PF_*_*',
          'keep *_jetBProbabilityBJetTagsSC5PF_*_*',
          'keep *_jetProbabilityBJetTagsSC5PF_*_*',
          'keep *_simpleSecondaryVertexBJetTagsSC5PF_*_*',
          'keep *_combinedSecondaryVertexBJetTagsSC5PF_*_*',
          'keep *_combinedSecondaryVertexMVABJetTagsSC5PF_*_*',
          'keep *_trackCountingHighEffBJetTagsSC7PF_*_*',
          'keep *_jetBProbabilityBJetTagsSC7PF_*_*',
          'keep *_jetProbabilityBJetTagsSC7PF_*_*',
          'keep *_simpleSecondaryVertexBJetTagsSC7PF_*_*',
          'keep *_combinedSecondaryVertexBJetTagsSC7PF_*_*',
          'keep *_combinedSecondaryVertexMVABJetTagsSC7PF_*_*'
    )
)

MyEventContent_edmDump = cms.PSet(
    outputCommands = cms.untracked.vstring(
          'keep *_trackMultiplicity_*_Analysis',
          'keep *_trackMultiplicityAssociatedToPV_*_Analysis',
          'keep *_trackMultiplicityOutsideJets_*_Analysis',
          'keep *_trackMultiplicityTransverseRegion_*_Analysis',
          'keep *_pileUpInfo_*_Analysis',
          'keep *_hfTower_*_Analysis',
          'keep *_xiTower_*_Analysis',
          'keep *_xiFromCaloTowers_*_Analysis'
    )
)

MyEventContent_extra = cms.PSet(
    outputCommands = cms.untracked.vstring(
          'keep *_EventAuxilary_*_*',
          'keep edmTriggerResults_TriggerResults_*_*',
          'keep *_hltTriggerSummaryAOD_*_*',
          'keep L1GlobalTriggerObjectMapRecord_*_*_*',
          'keep L1GlobalTriggerReadoutRecord_*_*_*',
          'keep *_hcalnoise_*_*',
          'keep *_genParticles_*_*',
          'keep recoTracks_generalTracks_*_*',
          'keep *_pixelVertices_*_*',
          'keep *_offlinePrimaryVertices_*_*',
          'keep *_particleFlow_*_*' 
    )
)

MyEventContent = cms.PSet(
        outputCommands = cms.untracked.vstring('drop *')
)
MyEventContent.outputCommands.extend(MyEventContent_tracks.outputCommands)
MyEventContent.outputCommands.extend(MyEventContent_jets.outputCommands)
MyEventContent.outputCommands.extend(MyEventContent_btag.outputCommands)
MyEventContent.outputCommands.extend(MyEventContent_edmDump.outputCommands)
MyEventContent.outputCommands.extend(MyEventContent_extra.outputCommands)
MyEventContent.outputCommands.extend(PatEventContent.outputCommands)

MyEventContent_reduced = cms.PSet(
        outputCommands = cms.untracked.vstring('drop *')
)
MyEventContent_reduced.outputCommands.extend(MyEventContent_tracks_reduced.outputCommands)
MyEventContent_reduced.outputCommands.extend(MyEventContent_jets_reduced.outputCommands)
MyEventContent_reduced.outputCommands.extend(MyEventContent_btag_reduced.outputCommands)
MyEventContent_reduced.outputCommands.extend(MyEventContent_edmDump.outputCommands)
MyEventContent_reduced.outputCommands.extend(MyEventContent_extra.outputCommands)
MyEventContent_reduced.outputCommands.extend(PatEventContent.outputCommands)

MyEventContent_expanded = cms.PSet(
        outputCommands = cms.untracked.vstring('drop *')
)
MyEventContent_expanded.outputCommands.extend(MyEventContent_tracks_expanded.outputCommands)
MyEventContent_expanded.outputCommands.extend(MyEventContent_jets_expanded.outputCommands)
MyEventContent_expanded.outputCommands.extend(MyEventContent_btag_expanded.outputCommands)
MyEventContent_expanded.outputCommands.extend(MyEventContent_edmDump.outputCommands)
MyEventContent_expanded.outputCommands.extend(MyEventContent_extra.outputCommands)
MyEventContent_expanded.outputCommands.extend(PatEventContent.outputCommands)
