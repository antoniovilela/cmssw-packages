import FWCore.ParameterSet.Config as cms

PatEventContent = cms.PSet(
    outputCommands = cms.untracked.vstring(
          'keep *_selectedLayer1Jets*_*_*',
          'keep *_layer1METs_*_*'
    )
)

MyEventContent_tracks = cms.PSet(
    outputCommands = cms.untracked.vstring(
          'keep *_selectGoodTracks_*_Analysis',
          'keep *_selectTracksAssociatedToPV_*_Analysis',
          'keep *_tracksOutsideJets_*_Analysis',
          'keep *_tracksTransverseRegion_*_Analysis' 
    )
)

MyEventContent_jets = cms.PSet(
    outputCommands = cms.untracked.vstring(
          'keep *_kt6CaloJets_*_*',
          'keep *_sisCone5CaloJets_*_*',
          'keep *_sisCone7CaloJets_*_*',
          'keep *_kt6PFJets_*_*',
          'keep *_sisCone5PFJets_*_*',
          'keep *_sisCone7PFJets_*_*',
          'keep *_L2L3CorJetKT6Calo_*_Analysis',
          'keep *_L2L3CorJetSC5Calo_*_Analysis',
          'keep *_L2L3CorJetSC7Calo_*_Analysis',
          'keep *_L2L3CorJetKT6PF_*_Analysis',
          'keep *_L2L3CorJetSC5PF_*_Analysis',
          'keep *_L2L3CorJetSC7PF_*_Analysis'
    )
)

MyEventContent_btag = cms.PSet(
    outputCommands = cms.untracked.vstring(
          'keep *_trackCountingHighEffBJetTags_*_*',
          'keep *_jetBProbabilityBJetTags_*_*',
          'keep *_jetProbabilityBJetTags_*_*',
          'keep *_simpleSecondaryVertexBJetTags_*_*',
          'keep *_combinedSecondaryVertexBJetTags_*_*',
          'keep *_combinedSecondaryVertexMVABJetTags_*_*' 
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
          'keep *_xiTower_*_Analysis' 
    )
)

MyEventContent = cms.PSet(
        outputCommands = cms.untracked.vstring('drop *')
)

MyEventContent.outputCommands.append('keep *_genParticles_*_*')
MyEventContent.outputCommands.append('keep recoTracks_generalTracks_*_*')
MyEventContent.outputCommands.append('keep *_offlinePrimaryVertices_*_*')
MyEventContent.outputCommands.append('keep *_particleFlow_*_*')

MyEventContent.outputCommands.extend(MyEventContent_tracks.outputCommands)
MyEventContent.outputCommands.extend(MyEventContent_jets.outputCommands)
MyEventContent.outputCommands.extend(MyEventContent_btag.outputCommands)
MyEventContent.outputCommands.extend(MyEventContent_edmDump.outputCommands)

MyEventContent.outputCommands.extend(PatEventContent.outputCommands)

output = cms.OutputModule("PoolOutputModule",
    MyEventContent,
    fileName = cms.untracked.string(''),
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('USER'),
        filterName = cms.untracked.string('')
    ),
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('')
    )
)
