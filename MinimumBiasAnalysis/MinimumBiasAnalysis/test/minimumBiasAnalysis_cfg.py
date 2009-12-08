import FWCore.ParameterSet.Config as cms

process = cms.Process("Analysis")

process.load('FWCore.MessageService.MessageLogger_cfi')

process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    #fileNames = cms.untracked.vstring('file:/data1/antoniov/MinimumBias_BeamCommissioning09_rereco_FIRSTCOLL_v1_RECO_LS34_EA91CC4D-28D9-DE11-833F-00261894396E.root')
    #fileNames = cms.untracked.vstring('file:/afs/cern.ch/cms/CAF/CMSCOMM/COMM_GLOBAL/Run122294_BSCSkim_MinBiasPD_ReTracking.root')
    #fileNames = cms.untracked.vstring('file:/afs/cern.ch/cms/CAF/CMSCOMM/COMM_GLOBAL/Run122314_BSCSkim_MinBiasPD_ReTracking.root')
    #fileNames = cms.untracked.vstring('/store/caf/user/malgeri/MinimumBias/rereco_TOBON_collisions_v3/8c9b06949bc7320e4d50bd1f4bcbe723/MinBias122314_9.root')
    fileNames = cms.untracked.vstring(
        #'/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/RecoTracks-335p3_v1/0099/6CCFFEA0-89E2-DE11-AE49-002618943809.root'
        #'/store/data/BeamCommissioning09/MinimumBias/RECO/v2/000/123/596/F67BCF17-48E2-DE11-98B1-000423D94534.root'
        #'/store/data/BeamCommissioning09/MinimumBias/RECO/rereco_GR09_P_V7_v1/0099/DABD5D6D-D4E2-DE11-8FFD-00261894387A.root'
        '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/RecoTracks-skim_GR09_P_V7_v1/0099/8EDF37E1-28E3-DE11-9C5D-001A92971B90.root'
    )
)
#/MinimumBias/BeamCommissioning09-RecoTracks-skim_GR09_P_V7_v1/RAW-RECO
process.source.fileNames = cms.untracked.vstring(
        '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/RecoTracks-skim_GR09_P_V7_v1/0099/F8572F0D-2DE3-DE11-A513-003048678BEA.root',
        '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/RecoTracks-skim_GR09_P_V7_v1/0099/F21B2F72-2DE3-DE11-B625-001A92810AEC.root',
        '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/RecoTracks-skim_GR09_P_V7_v1/0099/BE3FC46A-2DE3-DE11-AA56-0026189438D6.root',
        '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/RecoTracks-skim_GR09_P_V7_v1/0099/981F2C80-2DE3-DE11-ACA3-0018F3D09648.root',
        '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/RecoTracks-skim_GR09_P_V7_v1/0099/8EDF37E1-28E3-DE11-9C5D-001A92971B90.root',
        '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/RecoTracks-skim_GR09_P_V7_v1/0099/7E447E69-2DE3-DE11-86A8-001A92971B5E.root',
        '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/RecoTracks-skim_GR09_P_V7_v1/0099/6AFF1D7D-2DE3-DE11-BC36-0018F3D09648.root',
        '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/RecoTracks-skim_GR09_P_V7_v1/0099/6A86747C-2DE3-DE11-9C04-0018F3D09648.root',
        '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/RecoTracks-skim_GR09_P_V7_v1/0099/3EEF6EE3-2AE3-DE11-9DED-001A92971B90.root',
        '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/RecoTracks-skim_GR09_P_V7_v1/0099/1A37AA68-2DE3-DE11-ADA0-0018F3D096BC.root',
        '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/RecoTracks-skim_GR09_P_V7_v1/0099/0AE12E72-2DE3-DE11-BC7F-001A92810AEC.root',
        '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/RecoTracks-skim_GR09_P_V7_v1/0099/06F5171F-2DE3-DE11-96F8-003048678BEA.root',
        '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/RecoTracks-skim_GR09_P_V7_v1/0099/005BE17C-2DE3-DE11-90F1-0018F3D09648.root'
)

process.load("Configuration/StandardSequences/Geometry_cff")
process.load("Configuration/StandardSequences/MagneticField_cff")
process.load("Configuration/StandardSequences/FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = 'GR09_P_V8::All'

process.load("MinimumBiasAnalysis.MinimumBiasAnalysis.analysisSequences_cff")
process.load("RecoMET.Configuration.RecoMET_BeamHaloId_cff")

process.load("MinimumBiasAnalysis.MinimumBiasAnalysis.outputModule_cfi")
from MinimumBiasAnalysis.MinimumBiasAnalysis.minimumBiasEventContent_cff import MinimumBiasEventContent
process.output.outputCommands = MinimumBiasEventContent.outputCommands
#process.output.fileName = 'minimumBias.root'
process.output.fileName = '/tmp/antoniov/minimumBias_rereco_RecoTracks.root'
#process.output.SelectEvents.SelectEvents = cms.vstring('selection_step')
process.output.SelectEvents.SelectEvents = cms.vstring('*')
#process.output.SelectEvents.SelectEvents = cms.vstring('generalTracksPath','pixelTracksPath','pixelLessTracksPath')

#process.TFileService = cms.Service("TFileService",
#                                   fileName = cms.string("analysis_histos.root")
#)

##########
process.generalTracksFilter = cms.EDFilter("TrackCountFilter",
                                                      src = cms.InputTag('generalTracks'),
                                                      minNumber = cms.uint32(2) 
                                                      )
process.pixelLessTracksFilter = cms.EDFilter("TrackCountFilter",
                                                      src = cms.InputTag('ctfPixelLess'),
                                                      minNumber = cms.uint32(1) 
                                                      )
process.pixelTracksFilter = cms.EDFilter("TrackCountFilter",
                                                      src = cms.InputTag('pixelTracks'),
                                                      minNumber = cms.uint32(1) 
                                                      )

#process.generalTracksPath = cms.Path(process.generalTracksFilter)
#process.pixelTracksPath = cms.Path(process.pixelLessTracksFilter)
#process.pixelLessTracksPath = cms.Path(process.pixelTracksFilter)
#########

process.recoSequence = cms.Sequence(process.tracks*process.edmDump)

#process.selection_step = cms.Path(process.hlt)
#process.reco_step = cms.Path(process.hlt+process.recoSequence+process.BeamHaloId)
process.reco_step = cms.Path(process.recoSequence+process.BeamHaloId)

process.out_step = cms.EndPath(process.output)
#process.schedule = cms.Schedule(process.hlt_step,process.reco_step,process.analysis_step,process.out_step)
