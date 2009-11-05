import FWCore.ParameterSet.Config as cms

process = cms.Process("Analysis")

process.load('FWCore.MessageService.MessageLogger_cfi')
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('file:/tmp/antoniov/ExHuME_CEPDijetsGG_M100_10TeV_cff_py_RAW2DIGI_RECO.root')
)

process.load('Configuration/StandardSequences/GeometryPilot2_cff')
process.load('Configuration/StandardSequences/MagneticField_38T_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = 'MC_31X_V3::All'

process.load("PhysicsTools.PatAlgos.patSequences_cff")

import PhysicsTools.PatAlgos.tools.coreTools as coreTools
import PhysicsTools.PatAlgos.tools.jetTools as jetTools

#jetTools.switchJECSet(process,"Summer08Redigi","Summer08")

#jetAlgos = ['KT6','SC5','SC7']
#jetTypes = ['Calo','PF']

jetAlgos = ['SC5','SC7']
jetTypes = ['PF']

for algo in jetAlgos:
    for type in jetTypes:
        algo_full = (algo.replace('KT','kt')).replace('SC','sisCone')
        label = algo+type
        coll = algo_full+type+'Jets'
        genColl = algo_full+'GenJets'
        jetTools.addJetCollection(process,cms.InputTag(coll),label,
                                  doJTA=True,doBTagging=True,
                                  jetCorrLabel=(algo,type),
                                  doType1MET=False,doL1Cleaning=False,doL1Counters=False,
                                  genJetCollection=cms.InputTag(genColl))

jetTools.switchJetCollection(process,
                             cms.InputTag("sisCone7PFJets"),
                             doJTA=True,
                             doBTagging=True,
                             jetCorrLabel=('SC7','PF'),
                             doType1MET=False,
                             genJetCollection=cms.InputTag("sisCone7GenJets"))

from PhysicsTools.PatAlgos.patTemplate_cfg import process as processPATtemplate
process.out = processPATtemplate.out
objs = ['Muons','Electrons','Taus','Photons']
coreTools.removeSpecificPATObjects(process,objs)
coreTools.removeCleaning(process)

process.load("ExclusiveDijetsAnalysis.ExclusiveDijetsAnalysis.analysisSequences_cff")
#process.exclusiveDijetsHLTFilter.TriggerResultsTag = cms.InputTag("TriggerResults","","HLT8E29")
process.exclusiveDijetsHLTFilter.HLTPaths = ['HLT_DiJetAve15U_1E31']
#process.exclusiveDijetsHLTFilter.HLTPaths = ['HLT_DiJetAve30U_1E31']
#process.exclusiveDijetsHLTFilter.HLTPaths = ['HLT_DiJetAve30U_8E29']
#process.exclusiveDijetsHLTFilter.HLTPaths = ['HLT_Jet30U']

process.load("DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.pileUpInfo_cfi")
process.pileUpInfo.AccessCrossingFramePlayBack = True
process.pileUpInfo.BunchCrossings = cms.vint32(0)
process.load("DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.pileUpNumberFilter_cfi")

from ExclusiveDijetsAnalysis.ExclusiveDijetsAnalysis.myEventContent_cff import MyEventContent_PAT as MyEventContent
process.load("ExclusiveDijetsAnalysis.ExclusiveDijetsAnalysis.outputModule_cfi")
process.output.outputCommands = MyEventContent.outputCommands 
process.output.fileName = 'edmDump_exclusiveDijets.root'
process.output.SelectEvents.SelectEvents = cms.vstring('selection_step')

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("analysis_histos.root")
)

#process.recoSequence = cms.Sequence(process.jets*process.btagging*process.tracks*process.edmDump+process.pileUpInfo)
process.recoSequence = cms.Sequence(process.jets*process.tracks*process.edmDump+process.pileUpInfo)

process.selection_step = cms.Path(process.hlt)
process.reco_step = cms.Path(process.hlt+process.recoSequence)
process.pat_step = cms.Path(process.hlt+process.patDefaultSequence)
process.analysis_step = cms.Path(process.analysisBefore+
                                 process.analysisAfter)

process.out_step = cms.EndPath(process.output)
#process.schedule = cms.Schedule(process.hlt_step,process.reco_step,process.analysis_step,process.out_step)
