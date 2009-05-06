import FWCore.ParameterSet.Config as cms

process = cms.Process("Analysis")

process.load('FWCore.MessageService.MessageLogger_cfi')
process.MessageLogger.debugModules = cms.untracked.vstring('')
process.MessageLogger.cerr.threshold = 'INFO'
process.MessageLogger.cerr.INFO.limit = -1

process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('file:/tmp/antoniov/QCDpt30_Summer08_IDEAL_V11_redigi_v1_RECO_345194AC-4EE3-DD11-9EF6-001D0967D634.root')
)

process.load("ExclusiveDijetsAnalysis.ExclusiveDijetsAnalysis.analysisSequences_cff")

process.load("ExclusiveDijetsAnalysis.ExclusiveDijetsAnalysis.outputModule_cfi")
process.output.fileName = '/tmp/antoniov/edmDump_exclusiveDijets.root'
process.output.SelectEvents.SelectEvents = cms.vstring('selection_step')

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("analysis_histos.root")
)

process.selection_step = cms.Path(process.hlt)
process.reco_step = cms.Path(process.jets*process.tracks*process.edmDump)
process.analysis_step = cms.Path(process.analysisBefore+
                                 process.analysisAfter)

process.out_step = cms.EndPath(process.output)
#process.schedule = cms.Schedule(process.hlt_step,process.reco_step,process.analysis_step,process.out_step)
