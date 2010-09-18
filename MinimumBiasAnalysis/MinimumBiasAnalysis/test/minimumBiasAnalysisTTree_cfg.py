import FWCore.ParameterSet.Config as cms

# Settings
class config: pass
config.writeEdmOutput = False
config.runOnMC = False
config.globalTagNameData = 'GR_R_36X_V12A::All'
config.globalTagNameMC = 'START36_V10::All'
config.outputEdmFile = '/tmp/antoniov/minimumBias.root'
config.outputTTreeFile = '/tmp/antoniov/analysisMinBias_TTree_MinimumBias.root'
config.instLumiROOTFile = 'lumibylsXing_132440-144114_7TeV_StreamExpress_Collisions10_V2_sub_132440.root'
config.comEnergy = 7000.0
config.trackAnalyzerName = 'trackHistoAnalyzer'
#config.trackTagName = 'selectGoodTracks'
config.trackTagName = 'analysisTracks'
config.varyAttributes = True
config.runOfflineOnly = True
config.runNoColl = True
config.runBPTX = False
config.runHCALFilter = True

process = cms.Process("Analysis")

process.load('FWCore.MessageService.MessageLogger_cfi')
process.MessageLogger.cerr.threshold = 'INFO'

process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(3000) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        #'file:/tmp/antoniov/MinimumBias_Commissioning10_GOODCOLL-Jun14thSkim_v1_CA78CDED-2E83-DF11-BDD8-0026189438A7.root'
        'file:/tmp/antoniov/MinimumBias_Commissioning10_GOODCOLL-Jun14thSkim_v1_RECO_EC45524A-E682-DF11-B8A7-001A92810AAA.root'
    )
)
# Import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.GeometryExtended_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.Reconstruction_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
if config.runOnMC: process.GlobalTag.globaltag = config.globalTagNameMC
else: process.GlobalTag.globaltag = config.globalTagNameData

process.load("MinimumBiasAnalysis.MinimumBiasAnalysis.analysisSequences_cff")

if config.writeEdmOutput:
    process.load("MinimumBiasAnalysis.MinimumBiasAnalysis.outputModule_cfi")
    from MinimumBiasAnalysis.MinimumBiasAnalysis.minimumBiasEventContent_cff import MinimumBiasEventContent
    process.output.outputCommands = MinimumBiasEventContent.outputCommands
    process.output.fileName = config.outputEdmFile
    process.output.SelectEvents.SelectEvents = cms.vstring('selection_step')

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string(config.outputTTreeFile)
)

###################################################################################
# HCAL reflagging
# HF RecHit reflagger -- specify type of HF cleaning to use
from Utilities.AnalysisTools.addHcalReflagging import addHFReflagging,addHBHEReflagging
process = addHFReflagging(process,version=10,isMC=config.runOnMC)
# HBHE RecHit reflagger
process = addHBHEReflagging(process)

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

process.lumiWeight = cms.EDProducer("LuminosityWeightProducer",
    rootFileName = cms.string(config.instLumiROOTFile),
    prefix = cms.untracked.string("instLumi")
)

process.xiTower.comEnergy = config.comEnergy
process.xiFromCaloTowers.comEnergy = config.comEnergy
process.xiFromJets.comEnergy = config.comEnergy
process.recoSequence = cms.Sequence(process.tracks*process.edmDump+process.lumiWeight)
# Reflagging and re-reco
process.reflagging_step = cms.Path(process.hfrecoReflagged+process.hbherecoReflagged)
process.rereco_step = cms.Path(process.caloTowersRec
                               *(process.recoJets*process.recoJetIds+process.recoTrackJets)
                               *process.recoJetAssociations
                               *process.metreco
                               ) # re-reco jets and met

#process.l1Selection_step = cms.Path(process.l1CollBscOr)
#process.hltSelection_step = cms.Path(process.hltBscMinBiasOR)
process.selection_step = cms.Path(process.eventSelectionBscMinBiasOR)
process.reco_step = cms.Path(process.eventSelection+process.recoSequence)

if config.writeEdmOutput: process.out_step = cms.EndPath(process.output)

###################################################################################

# Analysis modules
process.load('MinimumBiasAnalysis.MinimumBiasAnalysis.minimumBiasTTreeAnalysis_cfi')
process.minimumBiasTTreeAnalysis.EBeam = config.comEnergy/2.
process.minimumBiasTTreeAnalysis.TrackTag = config.trackTagName
process.minimumBiasTTreeAnalysisNoCleaning = process.minimumBiasTTreeAnalysis.clone(
    CaloTowerTag = cms.InputTag("towerMaker::RECO")
)

process.load('Utilities.AnalysisTools.trackHistos_cfi')
process.trackHistos.src = config.trackTagName
process.load('Utilities.AnalysisTools.trackHistoAnalyzer_cfi')
process.trackHistoAnalyzer.TrackTag = config.trackTagName

attributesEnergyScale = [{'ApplyEnergyScaleHCAL':True,'EnergyScaleFactorHCAL':0.90,'HCALTowerSummaryTag':'hcalActivitySummaryScale090'},
                         {'ApplyEnergyScaleHCAL':True,'EnergyScaleFactorHCAL':0.92,'HCALTowerSummaryTag':'hcalActivitySummaryScale092'},
                         {'ApplyEnergyScaleHCAL':True,'EnergyScaleFactorHCAL':0.95,'HCALTowerSummaryTag':'hcalActivitySummaryScale095'},
                         {'ApplyEnergyScaleHCAL':True,'EnergyScaleFactorHCAL':0.98,'HCALTowerSummaryTag':'hcalActivitySummaryScale098'},  
                         {'ApplyEnergyScaleHCAL':True,'EnergyScaleFactorHCAL':1.02,'HCALTowerSummaryTag':'hcalActivitySummaryScale102'},
                         {'ApplyEnergyScaleHCAL':True,'EnergyScaleFactorHCAL':1.05,'HCALTowerSummaryTag':'hcalActivitySummaryScale105'},
                         {'ApplyEnergyScaleHCAL':True,'EnergyScaleFactorHCAL':1.08,'HCALTowerSummaryTag':'hcalActivitySummaryScale108'}, 
                         {'ApplyEnergyScaleHCAL':True,'EnergyScaleFactorHCAL':1.10,'HCALTowerSummaryTag':'hcalActivitySummaryScale110'}]

attributesThresholds = [{'EnergyThresholdHF':3.6},
                        {'EnergyThresholdHF':6.0},
                        {'EnergyThresholdHF':8.0},
                        {'EnergyThresholdHB':1.2,'EnergyThresholdHE':1.6},
                        {'EnergyThresholdHB':1.8,'EnergyThresholdHE':2.4}]

attributes = attributesEnergyScale
attributes.extend(attributesThresholds)

from Utilities.PyConfigTools.analysisTools import *

makeAnalysis(process,'minimumBiasTTreeAnalysis','eventSelectionBscMinBiasOR')
makeAnalysis(process,'minimumBiasTTreeAnalysisNoCleaning','eventSelectionBscMinBiasOR')
makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasOR')

if config.varyAttributes:
    makeAnalysis(process,'minimumBiasTTreeAnalysis','eventSelectionBscMinBiasOR',attributes)

if config.runOfflineOnly:
    makeAnalysis(process,'minimumBiasTTreeAnalysis','eventSelection')

if config.runBPTX:
    makeAnalysis(process,'minimumBiasTTreeAnalysis','eventSelectionBPTX')
    makeAnalysis(process,'minimumBiasTTreeAnalysis','eventSelectionL1Tech4')

if config.runNoColl:
    makeAnalysis(process,'minimumBiasTTreeAnalysis','eventSelectionBscMinBiasORNoColl')
    makeAnalysis(process,'minimumBiasTTreeAnalysis','eventSelectionBscMinBiasORBPTXOR')

if config.runHCALFilter:
    makeAnalysis(process,'minimumBiasTTreeAnalysis','eventSelectionBscMinBiasORHFVetoPlus')
    makeAnalysis(process,'minimumBiasTTreeAnalysis','eventSelectionBscMinBiasORHFVetoMinus')
    makeAnalysis(process,'minimumBiasTTreeAnalysis','eventSelectionBscMinBiasORHEHFVetoPlus')
    makeAnalysis(process,'minimumBiasTTreeAnalysis','eventSelectionBscMinBiasORHEHFVetoMinus')
    makeAnalysis(process,'minimumBiasTTreeAnalysis','eventSelectionBscMinBiasORSumEMaxHFPlus4')
    makeAnalysis(process,'minimumBiasTTreeAnalysis','eventSelectionBscMinBiasORSumEMaxHFPlus8')
    makeAnalysis(process,'minimumBiasTTreeAnalysis','eventSelectionBscMinBiasORSumEMaxHFPlus12')
    makeAnalysis(process,'minimumBiasTTreeAnalysis','eventSelectionBscMinBiasORSumEMaxHFPlus16')
    makeAnalysis(process,'minimumBiasTTreeAnalysis','eventSelectionBscMinBiasORSumEMaxHFMinus4')
    makeAnalysis(process,'minimumBiasTTreeAnalysis','eventSelectionBscMinBiasORSumEMaxHFMinus8')
    makeAnalysis(process,'minimumBiasTTreeAnalysis','eventSelectionBscMinBiasORSumEMaxHFMinus12')
    makeAnalysis(process,'minimumBiasTTreeAnalysis','eventSelectionBscMinBiasORSumEMaxHFMinus16')    

    makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasORHFVetoPlus')
    makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasORHFVetoMinus')
    makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasORHEHFVetoPlus')
    makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasORHEHFVetoMinus')
    makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasORSumEMaxHFPlus4')
    makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasORSumEMaxHFPlus8')
    makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasORSumEMaxHFPlus12')
    makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasORSumEMaxHFPlus16')
    makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasORSumEMaxHFMinus4')
    makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasORSumEMaxHFMinus8')
    makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasORSumEMaxHFMinus12')
    makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasORSumEMaxHFMinus16')
