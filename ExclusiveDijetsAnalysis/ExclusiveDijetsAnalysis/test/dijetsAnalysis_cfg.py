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

process.load("PhysicsTools.PatAlgos.patLayer0_cff")
process.load("PhysicsTools.PatAlgos.patLayer1_cff")

import PhysicsTools.PatAlgos.tools.jetTools as jetTools

#process.patBeforeLevel0Reco = cms.Sequence(process.patAODElectronIsolation*
#                                           process.patAODJetMETCorrections)

#process.patLayer0Cleaners = cms.Sequence(process.allLayer0Electrons*
#                                         process.allLayer0Jets*
#                                         process.allLayer0METs)

process.patBeforeLevel0Reco = cms.Sequence(process.patAODJetMETCorrections)

process.patLayer0Cleaners = cms.Sequence(process.allLayer0Jets*
                                         process.allLayer0METs)

process.patHighLevelReco = cms.Sequence(process.patJetFlavourId*
                                        process.patLayer0JetMETCorrections*
                                        process.patLayer0JetTracksCharge)

process.patMCTruth = cms.Sequence(process.patMCTruth_Jet)

process.patLayer0_withoutTrigMatch_new = cms.Sequence(process.patBeforeLevel0Reco*
                                                      process.patLayer0Cleaners*
                                                      process.patHighLevelReco*
                                                      process.patMCTruth)

process.patLayer0 = process.patLayer0_withoutTrigMatch_new

process.allLayer1Jets.addTrigMatch = False
#process.allLayer1Jets.trigPrimMatch = cms.VInputTag()

process.allLayer1METs.addTrigMatch = False

process.patLayer1 = cms.Sequence(process.layer1Jets*process.layer1METs)

jetTools.switchJetCollection(process,
                             "sisCone7PFJets",
                             [0,1],
                             None,
                             True,
                             False,
                             ('SC7','PF'),
                             False,
                             cms.InputTag("sisCone7GenJets")) 

#process.load("ExclusiveDijetsAnalysis.ExclusiveDijetsAnalysis.analysisSequences_cff")
process.load("ExclusiveDijetsAnalysis.ExclusiveDijetsAnalysis.analysisSequences_expanded_cff")

process.load("DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.pileUpInfo_cfi")
process.load("DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.pileUpNumberFilter_cfi")

#process.load("ExclusiveDijetsAnalysis.ExclusiveDijetsAnalysis.outputModule_cfi")
process.load("ExclusiveDijetsAnalysis.ExclusiveDijetsAnalysis.outputModule_expanded_cfi")
process.output.fileName = '/tmp/antoniov/edmDump_exclusiveDijets.root'
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
