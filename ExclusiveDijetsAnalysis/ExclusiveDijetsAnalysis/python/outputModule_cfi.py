import FWCore.ParameterSet.Config as cms

MyEventContent = cms.PSet(
        outputCommands = cms.untracked.vstring('drop *')
)
MyEventContent.outputCommands.append('keep *_selectGoodTracks_*_Analysis')
MyEventContent.outputCommands.append('keep *_selectTracksAssociatedToPV_*_Analysis')
MyEventContent.outputCommands.append('keep *_tracksOutsideJets_*_Analysis')
MyEventContent.outputCommands.append('keep *_tracksTransverseRegion_*_Analysis')
MyEventContent.outputCommands.append('keep *_sisCone7PFJets_*_*')
MyEventContent.outputCommands.append('keep *_L2L3CorJetSC7PF_*_Analysis')
MyEventContent.outputCommands.append('keep *_trackMultiplicity_*_Analysis')
MyEventContent.outputCommands.append('keep *_trackMultiplicityAssociatedToPV_*_Analysis')
MyEventContent.outputCommands.append('keep *_trackMultiplicityOutsideJets_*_Analysis')
MyEventContent.outputCommands.append('keep *_trackMultiplicityTransverseRegion_*_Analysis')
MyEventContent.outputCommands.append('keep *_pileUpInfo_*_Analysis')
MyEventContent.outputCommands.append('keep *_hfTower_*_Analysis')
MyEventContent.outputCommands.append('keep *_xiTower_*_Analysis')
MyEventContent.outputCommands.append('keep *_genParticles_*_*')
MyEventContent.outputCommands.append('keep recoTracks_generalTracks_*_*')
MyEventContent.outputCommands.append('keep *_offlinePrimaryVertices_*_*')
MyEventContent.outputCommands.append('keep *_particleFlow_*_*')

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
