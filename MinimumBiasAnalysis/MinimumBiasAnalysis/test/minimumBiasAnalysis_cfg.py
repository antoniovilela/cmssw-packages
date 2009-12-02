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
        '/store/caf/user/malgeri/MinimumBias/rereco_TOBON_collisions_v3/8c9b06949bc7320e4d50bd1f4bcbe723/MinBias122314_9.root',
        '/store/caf/user/malgeri/MinimumBias/rereco_TOBON_collisions_v3/8c9b06949bc7320e4d50bd1f4bcbe723/MinBias122314_8.root',
        '/store/caf/user/malgeri/MinimumBias/rereco_TOBON_collisions_v3/8c9b06949bc7320e4d50bd1f4bcbe723/MinBias122314_7.root',
        '/store/caf/user/malgeri/MinimumBias/rereco_TOBON_collisions_v3/8c9b06949bc7320e4d50bd1f4bcbe723/MinBias122314_6.root',
        '/store/caf/user/malgeri/MinimumBias/rereco_TOBON_collisions_v3/8c9b06949bc7320e4d50bd1f4bcbe723/MinBias122314_5.root',
        '/store/caf/user/malgeri/MinimumBias/rereco_TOBON_collisions_v3/8c9b06949bc7320e4d50bd1f4bcbe723/MinBias122314_4.root',
        '/store/caf/user/malgeri/MinimumBias/rereco_TOBON_collisions_v3/8c9b06949bc7320e4d50bd1f4bcbe723/MinBias122314_3.root',
        '/store/caf/user/malgeri/MinimumBias/rereco_TOBON_collisions_v3/8c9b06949bc7320e4d50bd1f4bcbe723/MinBias122314_2.root',
        '/store/caf/user/malgeri/MinimumBias/rereco_TOBON_collisions_v3/8c9b06949bc7320e4d50bd1f4bcbe723/MinBias122314_16.root',
        '/store/caf/user/malgeri/MinimumBias/rereco_TOBON_collisions_v3/8c9b06949bc7320e4d50bd1f4bcbe723/MinBias122314_15.root',
        '/store/caf/user/malgeri/MinimumBias/rereco_TOBON_collisions_v3/8c9b06949bc7320e4d50bd1f4bcbe723/MinBias122314_14.root',
        '/store/caf/user/malgeri/MinimumBias/rereco_TOBON_collisions_v3/8c9b06949bc7320e4d50bd1f4bcbe723/MinBias122314_13.root',
        '/store/caf/user/malgeri/MinimumBias/rereco_TOBON_collisions_v3/8c9b06949bc7320e4d50bd1f4bcbe723/MinBias122314_12.root',
        '/store/caf/user/malgeri/MinimumBias/rereco_TOBON_collisions_v3/8c9b06949bc7320e4d50bd1f4bcbe723/MinBias122314_11.root',
        '/store/caf/user/malgeri/MinimumBias/rereco_TOBON_collisions_v3/8c9b06949bc7320e4d50bd1f4bcbe723/MinBias122314_10.root',
        '/store/caf/user/malgeri/MinimumBias/rereco_TOBON_collisions_v3/8c9b06949bc7320e4d50bd1f4bcbe723/MinBias122314_1.root'
    )
)

process.load("Configuration/StandardSequences/Geometry_cff")
process.load("Configuration/StandardSequences/MagneticField_cff")
process.load("Configuration/StandardSequences/FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = 'GR09_P_V6::All'

process.load("MinimumBiasAnalysis.MinimumBiasAnalysis.analysisSequences_cff")
process.load("RecoMET.Configuration.RecoMET_BeamHaloId_cff")

process.load("MinimumBiasAnalysis.MinimumBiasAnalysis.outputModule_cfi")
from MinimumBiasAnalysis.MinimumBiasAnalysis.minimumBiasEventContent_cff import MinimumBiasEventContent
process.output.outputCommands = MinimumBiasEventContent.outputCommands
#process.output.fileName = 'minimumBias.root'
#process.output.fileName = 'minimumBias_Run122294.root'
#process.output.fileName = 'minimumBias_Run122314.root'
process.output.fileName = '/data1/antoniov/minimumBias_rereco_TOBON_collisions.root'
process.output.SelectEvents.SelectEvents = cms.vstring('selection_step')

#process.TFileService = cms.Service("TFileService",
#                                   fileName = cms.string("analysis_histos.root")
#)

process.recoSequence = cms.Sequence(process.tracks*process.edmDump)

process.selection_step = cms.Path(process.hlt)
process.reco_step = cms.Path(process.hlt+process.recoSequence+process.BeamHaloId)

process.out_step = cms.EndPath(process.output)
#process.schedule = cms.Schedule(process.hlt_step,process.reco_step,process.analysis_step,process.out_step)
