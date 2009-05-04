import FWCore.ParameterSet.Config as cms

process = cms.Process("Analysis")

process.load('FWCore.MessageService.MessageLogger_cfi')
process.MessageLogger.debugModules = cms.untracked.vstring('')
process.MessageLogger.cerr.threshold = 'INFO'
process.MessageLogger.cerr.INFO.limit = -1

process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('file:/tmp/antoniov/ExHuME_CEPDijetsGG_M100_10TeV_cff_py_RAW2DIGI_RECO_1.root')
)

process.load("ExclusiveDijetsAnalysis.ExclusiveDijetsAnalysis.exclusiveDijetsHLTPaths_cfi")

process.load("JetMETCorrections.Configuration.L2L3Corrections_Summer08_cff")
#process.prefer("L2L3JetCorrectorSC7PF")

process.load("ExclusiveDijetsAnalysis.ExclusiveDijetsAnalysis.leadingJets_cfi")
process.leadingJets.src = "L2L3CorJetSC7PF"

process.load("DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.selectGoodTracks_cfi")
process.selectGoodTracks.cut = "pt > 0.5 & numberOfValidHits > 7 & d0 <= 3.5"

process.load("DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.selectTracksAssociatedToPV_cfi")
process.selectTracksAssociatedToPV.src = "selectGoodTracks"
process.selectTracksAssociatedToPV.MaxDistanceFromVertex = 0.2

process.load("ExclusiveDijetsAnalysis.ExclusiveDijetsAnalysis.tracksOutsideJets_cfi")
process.tracksOutsideJets.src = "selectTracksAssociatedToPV" 
process.tracksOutsideJets.JetTag = "leadingJets"
process.tracksOutsideJets.JetConeSize = 0.7

process.load("ExclusiveDijetsAnalysis.ExclusiveDijetsAnalysis.tracksTransverseRegion_cfi")
process.tracksTransverseRegion.src = "selectTracksAssociatedToPV"
process.tracksTransverseRegion.JetTag = "leadingJets"

process.load("DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.trackMultiplicity_cfi") 
process.trackMultiplicity.TracksTag = "selectGoodTracks"
process.trackMultiplicityAssociatedToPV = process.trackMultiplicity.clone(TracksTag = "selectTracksAssociatedToPV")
process.trackMultiplicityOutsideJets = process.trackMultiplicity.clone(TracksTag = "tracksOutsideJets")
process.trackMultiplicityTransverseRegion = process.trackMultiplicity.clone(TracksTag = "tracksTransverseRegion")

process.load("DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.pileUpInfo_cfi")
process.load("DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.pileUpNumberFilter_cfi")

process.load("DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.hfTower_cfi")
process.load("DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.xiTower_cfi")
process.xiTower.UseMETInfo = False

process.l1filter = cms.EDFilter("L1TriggerTestFilter",
  HFRingETSumThreshold = cms.int32(0),
  L1TriggerNames = cms.vstring("L1_SingleJet30"),
  AccessL1GctHFRingEtSums = cms.untracked.bool(True) 
)

process.analysisBeforeSelection = cms.EDAnalyzer("SimpleDijetsAnalyzer",
    JetTag = cms.InputTag("L2L3CorJetSC7PF")
)
process.analysisAfterSelection = process.analysisBeforeSelection.clone()

process.MyEventContent = cms.PSet(
        outputCommands = cms.untracked.vstring('drop *')
)
process.MyEventContent.outputCommands.append('keep *_selectGoodTracks_*_Analysis')
process.MyEventContent.outputCommands.append('keep *_selectTracksAssociatedToPV_*_Analysis')
process.MyEventContent.outputCommands.append('keep *_tracksOutsideJets_*_Analysis')
process.MyEventContent.outputCommands.append('keep *_tracksTransverseRegion_*_Analysis')
process.MyEventContent.outputCommands.append('keep *_sisCone7PFJets_*_*')
process.MyEventContent.outputCommands.append('keep *_L2L3CorJetSC7PF_*_Analysis')
process.MyEventContent.outputCommands.append('keep *_trackMultiplicity_*_Analysis')
process.MyEventContent.outputCommands.append('keep *_trackMultiplicityAssociatedToPV_*_Analysis')
process.MyEventContent.outputCommands.append('keep *_trackMultiplicityOutsideJets_*_Analysis')
process.MyEventContent.outputCommands.append('keep *_trackMultiplicityTransverseRegion_*_Analysis')
process.MyEventContent.outputCommands.append('keep *_pileUpInfo_*_Analysis')
process.MyEventContent.outputCommands.append('keep *_hfTower_*_Analysis')
process.MyEventContent.outputCommands.append('keep *_xiTower_*_Analysis')
process.MyEventContent.outputCommands.append('keep *_genParticles_*_*')
process.MyEventContent.outputCommands.append('keep recoTracks_generalTracks_*_*')
process.MyEventContent.outputCommands.append('keep *_offlinePrimaryVertices_*_*')
process.MyEventContent.outputCommands.append('keep *_particleFlow_*_*')

process.output = cms.OutputModule("PoolOutputModule",
    process.MyEventContent,
    fileName = cms.untracked.string('edmDump_CEPDijetsGG_10TeV.root'),
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('USER'),
        filterName = cms.untracked.string('')
    ),
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('selection_step')
    )
)

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("analysis_histos.root")
)

process.hlt = cms.Sequence(process.l1filter + process.exclusiveDijetsHLTFilter)
process.jets = cms.Sequence(process.L2L3CorJetSC7PF*process.leadingJets)
process.tracks = cms.Sequence(process.selectGoodTracks*
                              process.selectTracksAssociatedToPV*
                              process.tracksOutsideJets+
                              process.tracksTransverseRegion) 
process.edmDump = cms.Sequence(process.trackMultiplicity+
                               process.trackMultiplicityAssociatedToPV+
                               process.trackMultiplicityOutsideJets+
                               process.trackMultiplicityTransverseRegion+
                               process.pileUpInfo+
                               process.hfTower+
                               process.xiTower)
process.analysisBefore = cms.Sequence(process.analysisBeforeSelection)
process.analysisAfter = cms.Sequence(process.hlt*process.analysisAfterSelection)

process.selection_step = cms.Path(process.hlt)
process.reco_step = cms.Path(process.jets*process.tracks*process.edmDump)
process.analysis_step = cms.Path(process.analysisBefore+
                                 process.analysisAfter)

process.out_step = cms.EndPath(process.output)
#process.schedule = cms.Schedule(process.hlt_step,process.reco_step,process.analysis_step,process.out_step)
