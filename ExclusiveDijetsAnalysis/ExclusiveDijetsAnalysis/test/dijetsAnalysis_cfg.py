import FWCore.ParameterSet.Config as cms
import sys

print sys.argv

inputFields = ('fileIn','fileOut','accessPileUpInfo','reweightHFTower','reweightFileName','reweightHistoName')

class input: pass
for item in sys.argv:
    option = item.split('=')[0]
    if option in inputFields:
        value = item.split('=')[1]
        if value in ('true','True','yes','Yes'): value = True
        elif value in ('false','False','no','No'): value = False
        
        setattr(input,option,value)
        #print "Setting",option,"=",getattr(input,option)

requiredFields = ()
for item in requiredFields:
    if not hasattr(input,item):
        raise RuntimeError,'Need to set "%s"' % item

# Treat default case
if not hasattr(input,'fileIn'):
    input.fileIn = 'file:pool.root'
else:
    if input.fileIn.find(':') == -1: input.fileIn = 'file:' + input.fileIn

if not hasattr(input,'fileOut'): input.fileOut = 'edmDump_exclusiveDijets.root'
if not hasattr(input,'accessPileUpInfo'): input.accessPileUpInfo = False
if not hasattr(input,'reweightHFTower'): input.reweightHFTower = False
if not hasattr(input,'reweightFileName'): input.reweightFileName = 'reweightHistos.root'
if not hasattr(input,'reweightHistoName'): input.reweightHistoName = 'energyHFplusRatio'

# Print parameters
#for attr in input.__dict__:
#    print "Using %s = %s" % (attr,input.__dict__[attr])
for item in inputFields:
    print "Using %s = %s" % (item,getattr(input,item))

#jetAlgos = ['KT4','KT6','SC5','SC7']
#jetTypes = ['Calo','PF']
jetAlgos = ['SC5','SC7']
jetTypes = ['PF']

# Build cms.Process
process = cms.Process("Analysis")

process.load('FWCore.MessageService.MessageLogger_cfi')
process.MessageLogger.debugModules = cms.untracked.vstring('')
process.MessageLogger.cerr.threshold = 'INFO'
process.MessageLogger.categories.append('PATSummaryTables')
process.MessageLogger.cerr.INFO = cms.untracked.PSet(
    default          = cms.untracked.PSet( limit = cms.untracked.int32(0)  ),
    PATSummaryTables = cms.untracked.PSet( limit = cms.untracked.int32(-1) )
)

process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(input.fileIn)
)

process.load('Configuration/StandardSequences/GeometryPilot2_cff')
process.load('Configuration/StandardSequences/MagneticField_38T_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = 'IDEAL_V12::All'

# PAT
process.load("PhysicsTools.PatAlgos.patSequences_cff")

import PhysicsTools.PatAlgos.tools.coreTools as coreTools
import PhysicsTools.PatAlgos.tools.jetTools as jetTools

#jetTools.switchJECSet(process,"Summer08Redigi","Summer08")

for algo in jetAlgos:
    for type in jetTypes:
        algo_full = (algo.replace('KT','kt')).replace('SC','sisCone')
        label = algo+type
        coll = algo_full+type+'Jets'
        genColl = algo_full+'GenJets'
        jetTools.addJetCollection(process,cms.InputTag(coll),label,
                                  doJTA=True,doBTagging=True,
                                  jetCorrLabel=(algo,type),
                                  doType1MET=False,doL1Counters=False,
                                  genJetCollection=cms.InputTag(genColl),
                                  doTrigMatch=False)

jetTools.switchJetCollection(process,
                             cms.InputTag("sisCone7PFJets"),
                             doJTA=True,
                             doBTagging=True,
                             jetCorrLabel=('SC7','PF'),
                             doType1MET=False,
                             genJetCollection=cms.InputTag("sisCone7GenJets"))

objs = ['Muons','Electrons','Taus','Photons']
for item in objs: coreTools.removeSpecificPATObject(process,item)

coreTools.removeCleaning(process)

# Other analysis sequences
process.load("ExclusiveDijetsAnalysis.ExclusiveDijetsAnalysis.analysisSequences_cff")

if input.accessPileUpInfo:
    process.load("DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.pileUpInfo_cfi")
    process.pileUpInfo.AccessCrossingFramePlayBack = True
    process.pileUpInfo.BunchCrossings = cms.vint32(0)
    #process.load("DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.pileUpNumberFilter_cfi")

if input.reweightHFTower:
    process.hfTower.ReweightHFTower = True
    process.hfTower.ReweightHistoName = cms.vstring(input.reweightFileName,input.reweightHistoName)

#from ExclusiveDijetsAnalysis.ExclusiveDijetsAnalysis.myEventContent_cff import MyEventContent_PAT as MyEventContent
from ExclusiveDijetsAnalysis.ExclusiveDijetsAnalysis.myEventContent_cff import MyEventContent_expanded as MyEventContent
process.load("ExclusiveDijetsAnalysis.ExclusiveDijetsAnalysis.outputModule_cfi")
process.output.outputCommands = MyEventContent.outputCommands 
process.output.fileName = input.fileOut
process.output.SelectEvents.SelectEvents = cms.vstring('selection_step')

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("analysis_histos.root")
)

#process.recoSequence = cms.Sequence(process.jets*process.btagging*process.tracks*process.edmDump+process.pileUpInfo)
if input.accessPileUpInfo: process.recoSequence = cms.Sequence(process.jets*process.tracks*process.edmDump+process.pileUpInfo)
else: process.recoSequence = cms.Sequence(process.jets*process.tracks*process.edmDump)

process.selection_step = cms.Path(process.hlt)
process.reco_step = cms.Path(process.hlt+process.recoSequence)
process.pat_step = cms.Path(process.hlt+process.patDefaultSequence)
process.analysis_step = cms.Path(process.analysisBefore+
                                 process.analysisAfter)

process.out_step = cms.EndPath(process.output)
#process.schedule = cms.Schedule(process.hlt_step,process.reco_step,process.analysis_step,process.out_step)
