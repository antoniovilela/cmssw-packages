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
process.load("ExclusiveDijetsAnalysis.ExclusiveDijetsAnalysis.tracksOutsideJets_cfi")
process.tracksOutsideJets.src = "selectGoodTracks" 
process.tracksOutsideJets.JetTag = "leadingJets"
process.load("DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.selectTracksAssociatedToPV_cfi")
process.selectTracksAssociatedToPV.src = "tracksOutsideJets"
process.selectTracksAssociatedToPV.MaxDistanceFromVertex = 0.2

process.load("DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.trackMultiplicity_cfi") 
process.trackMultiplicity.TracksTag = "selectGoodTracks"
process.trackMultiplicityOutsideJets = process.trackMultiplicity.clone(TracksTag = "tracksOutsideJets")
process.trackMultiplicityAssociatedToPV = process.trackMultiplicity.clone(TracksTag = "selectTracksAssociatedToPV")

process.load("DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.pileUpInfo_cfi")
process.load("DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.pileUpNumberFilter_cfi")

process.analysis = cms.EDAnalyzer("SimpleDijetsAnalyzer",
    JetTag = cms.InputTag("L2L3CorJetSC7PF")
)

process.MyEventContent = cms.PSet(
        outputCommands = cms.untracked.vstring('drop *')
)
process.MyEventContent.outputCommands.append('keep *_selectGoodTracks_*_Analysis')
process.MyEventContent.outputCommands.append('keep *_tracksOutsideJets_*_Analysis')
process.MyEventContent.outputCommands.append('keep *_selectTracksAssociatedToPV_*_Analysis')
process.MyEventContent.outputCommands.append('keep *_iterativeCone5PFJets_*_*')
process.MyEventContent.outputCommands.append('keep *_L2L3CorJetSC7PF_*_Analysis')
process.MyEventContent.outputCommands.append('keep *_trackMultiplicity_*_Analysis')
process.MyEventContent.outputCommands.append('keep *_trackMultiplicityOutsideJets_*_Analysis')
process.MyEventContent.outputCommands.append('keep *_trackMultiplicityAssociatedToPV_*_Analysis')
process.MyEventContent.outputCommands.append('keep *_pileUpInfo_*_Analysis')
#process.MyEventContent.outputCommands.append('keep recoMuons_muons_*_*')
#process.MyEventContent.outputCommands.append('keep recoTracks_generalTracks_*_*')
#process.MyEventContent.outputCommands.append('keep *_offlinePrimaryVertices_*_*')
#process.MyEventContent.outputCommands.append('keep *_offlinePrimaryVerticesWithBS_*_*')
#process.MyEventContent.outputCommands.append('keep *_pixelVertices_*_*')

process.output = cms.OutputModule("PoolOutputModule",
    process.MyEventContent,
    fileName = cms.untracked.string('/tmp/antoniov/edmDump_CEPDijetsGG_M100_10TeV.root'),
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

process.hlt = cms.Sequence(process.exclusiveDijetsHLTFilter)
process.jets = cms.Sequence(process.L2L3CorJetSC7PF*process.leadingJets)
process.tracks = cms.Sequence(process.selectGoodTracks*process.tracksOutsideJets*process.selectTracksAssociatedToPV) 
process.edmDump = cms.Sequence(process.trackMultiplicity+
                               process.trackMultiplicityOutsideJets+
                               process.trackMultiplicityAssociatedToPV+
                               process.pileUpInfo)

process.selection_step = cms.Path(process.hlt)
process.reco_step = cms.Path(process.hlt*process.jets*process.tracks*process.edmDump)
process.analysis_step = cms.Path(process.hlt*process.analysis)

process.out_step = cms.EndPath(process.output)
#process.schedule = cms.Schedule(process.hlt_step,process.reco_step,process.analysis_step,process.out_step)

