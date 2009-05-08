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

process.load('Configuration/StandardSequences/GeometryPilot2_cff')
process.load('Configuration/StandardSequences/MagneticField_38T_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = 'IDEAL_V12::All'

process.load("ExclusiveDijetsAnalysis.ExclusiveDijetsAnalysis.patSequences_cff")

import PhysicsTools.PatAlgos.tools.jetTools as jetTools

jetTools.switchJetCollection(process,
                             "sisCone7PFJets",
                             layers=[0,1],
                             runCleaner=None,
                             doJTA=True,
                             doBTagging=False,
                             jetCorrLabel=('SC7','PF'),
                             doType1MET=False,
                             genJetCollection=cms.InputTag("sisCone7GenJets")) 

#jetTools.switchJECSet(process,"Summer08Redigi","Summer08")

#process.load("ExclusiveDijetsAnalysis.ExclusiveDijetsAnalysis.analysisSequences_cff")
process.load("ExclusiveDijetsAnalysis.ExclusiveDijetsAnalysis.analysisSequences_expanded_cff")

process.load("DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.pileUpInfo_cfi")
process.load("DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.pileUpNumberFilter_cfi")

#process.load("ExclusiveDijetsAnalysis.ExclusiveDijetsAnalysis.outputModule_cfi")
process.load("ExclusiveDijetsAnalysis.ExclusiveDijetsAnalysis.outputModule_expanded_cfi")
process.output.fileName = 'edmDump_exclusiveDijets.root'
process.output.SelectEvents.SelectEvents = cms.vstring('selection_step')

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("analysis_histos.root")
)

process.selection_step = cms.Path(process.hlt)
process.reco_step = cms.Path(process.jets*process.tracks*process.edmDump+process.pileUpInfo)
process.pat_step = cms.Path(process.patLayer0*process.patLayer1)
process.analysis_step = cms.Path(process.analysisBefore+
                                 process.analysisAfter)

process.out_step = cms.EndPath(process.output)
#process.schedule = cms.Schedule(process.hlt_step,process.reco_step,process.analysis_step,process.out_step)
