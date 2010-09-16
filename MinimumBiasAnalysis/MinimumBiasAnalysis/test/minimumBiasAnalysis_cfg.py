import FWCore.ParameterSet.Config as cms

process = cms.Process("Analysis")

process.load('FWCore.MessageService.MessageLogger_cfi')
process.MessageLogger.cerr.threshold = 'INFO'

process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(50000) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        #'/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/RecoTracks-skim_GR09_P_V7_v1/0099/8EDF37E1-28E3-DE11-9C5D-001A92971B90.root'
        '/store/data/BeamCommissioning09/MinimumBias/RECO/v2/000/124/030/E6A8AB2F-78E7-DE11-973F-001617C3B710.root'
    )
)

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.GeometryExtended_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.Reconstruction_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = ''

process.load("MinimumBiasAnalysis.MinimumBiasAnalysis.analysisSequences_cff")
#process.xiTower.comEnergy = 2360.0
#process.xiFromCaloTowers.comEnergy = 2360.0
#process.xiFromJets.comEnergy = 2360.0

#process.load("RecoMET.Configuration.RecoMET_BeamHaloId_cff")

process.load("MinimumBiasAnalysis.MinimumBiasAnalysis.outputModule_cfi")
from MinimumBiasAnalysis.MinimumBiasAnalysis.minimumBiasEventContent_cff import MinimumBiasEventContent
process.output.outputCommands = MinimumBiasEventContent.outputCommands
process.output.fileName = 'minimumBias.root'
process.output.SelectEvents.SelectEvents = cms.vstring('selection_step')

#process.TFileService = cms.Service("TFileService",
#                                   fileName = cms.string("analysisMinBias_histos.root")
#)

###################################################################################
# HCAL reflagging
# HF RecHit reflagger -- specify type of HF cleaning to use
process.load("JetMETAnalysis/HcalReflagging/HFrechitreflaggerJETMET_cff")
version = 10
isMC = False
if version==1:
    process.hfrecoReflagged = process.HFrechitreflaggerJETMETv1.clone()
elif version==2:
    process.hfrecoReflagged = process.HFrechitreflaggerJETMETv2.clone()
elif version==3:
    process.hfrecoReflagged = process.HFrechitreflaggerJETMETv3.clone()
elif version==4: 
    if (isMC==False):
        process.hfrecoReflagged = process.HFrechitreflaggerJETMETv4.clone()
    else:
        process.hfrecoReflagged = process.HFrechitreflaggerJETMETv2.clone()  
elif version==5:
    if (isMC==False):
        process.hfrecoReflagged = process.HFrechitreflaggerJETMETv5.clone()
    else:
        process.hfrecoReflagged = process.HFrechitreflaggerJETMETv3.clone()  
# CURRENT RECOMMENDATION
elif version==10:
    process.hfrecoReflagged = process.HFrechitreflaggerJETMETv10.clone()
    if (isMC==False):  # V10 cleaning uses results of prior flags when setting new flags; this is the current recommendation as of 21 July 2010 
        import string
        process.hfrecoReflagged.PETstat.flagsToSkip =string.atoi('10',2)
        process.hfrecoReflagged.S8S1stat.flagsToSkip=string.atoi('10010',2)
        process.hfrecoReflagged.S9S1stat.flagsToSkip=string.atoi('11010',2)
        # Flag ordering
        process.hfrecoReflagged.FlagsToSet=(4,3,0)  # set flag 4 (HFPET -- also sets HFLongShort), then flag 3 (HFS8S1 -- also sets HFLongShort), then flag 0 (HFLongShort -- set directly via S9S1)
# Add debugging here
#process.hfrecoReflagged.debug=1
import JetMETAnalysis.HcalReflagging.RemoveAddSevLevel as RemoveAddSevLevel

process.hcalRecAlgos=RemoveAddSevLevel.AddFlag(process.hcalRecAlgos,"HFLongShort",11)
if (isMC==False):  # Don't use HFDigiTime on MC !
    process.hcalRecAlgos=RemoveAddSevLevel.AddFlag(process.hcalRecAlgos,"HFDigiTime",11)

# HBHE RecHit reflagger
process.load("JetMETAnalysis/HcalReflagging/isolhbherechitreflaggerJETMET_cfi")
process.hbherecoReflagged = process.isolhbherechitreflaggerJETMET.clone()

process.hcalRecAlgos=RemoveAddSevLevel.AddFlag(process.hcalRecAlgos,"UserDefinedBit0",10)

print "These are the severity levels for the various rechit flags:"
print "(Severity > 10 causes rechit to be ignored by CaloTower maker)"
for i in process.hcalRecAlgos.SeverityLevels: print i

# Use the reflagged HF RecHits to make the CaloTowers
process.towerMaker.hfInput = "hfrecoReflagged"
process.towerMakerWithHO.hfInput = "hfrecoReflagged"
# Use the reflagged HBHE RecHits to make the CaloTowers
process.towerMaker.hbheInput = "hbherecoReflagged"
process.towerMakerWithHO.hbheInput = "hbherecoReflagged"

###################################################################################

process.recoSequence = cms.Sequence(process.tracks*process.edmDump)

"""
hltPaths = ('hltMinBiasBSCOR',
            'hltMinBiasPixel',
            'hltMinBiasBSCORNoColl',
            'hltMinBiasPixelNoColl',
            'hltMinBiasBSCORNoBPTX',
            'hltMinBiasPixelNoBPTX')
from Utilities.PyConfigTools.analysisTools import addPath
for path in hltPaths:
    addPath(process,getattr(process,path))
"""

# Reflagging and re-reco
process.reflagging_step = cms.Path(process.hfrecoReflagged+process.hbherecoReflagged)
process.rereco_step = cms.Path(process.caloTowersRec
                               *(process.recoJets*process.recoJetIds+process.recoTrackJets)
                               *process.recoJetAssociations
                               *process.metreco
                               ) # re-reco jets and met

process.selection_step = cms.Path(process.eventSelectionBscMinBiasOR)
process.reco_step = cms.Path(process.eventSelection+process.recoSequence)

process.out_step = cms.EndPath(process.output)
#process.schedule = cms.Schedule(process.selection_step,process.reco_step,process.analysis_step,process.out_step)
