import FWCore.ParameterSet.Config as cms

# Settings
class config: pass
config.comEnergy = 7000.0
config.varyAttributes = False
config.runOfflineOnly = True
config.runNoColl = False
config.runBPTX = False
config.runHCALFilter = True

# Skim sequences
from minimumBiasAnalysis_cfg import process
if hasattr(process,'output'): del process.output
if hasattr(process,'out_step'): del process.out_step
process.MessageLogger.cerr.threshold = 'INFO'
process.maxEvents.input = 20000
process.source.fileNames = ['file:/tmp/antoniov/MinimumBias_Commissioning10_GOODCOLL-Jun14thSkim_v1_CA78CDED-2E83-DF11-BDD8-0026189438A7.root']
process.GlobalTag.globaltag = 'GR_R_36X_V12A::All'
process.xiTower.comEnergy = config.comEnergy
process.xiFromCaloTowers.comEnergy = config.comEnergy
process.xiFromJets.comEnergy = config.comEnergy

process.load('MinimumBiasAnalysis.MinimumBiasAnalysis.minimumBiasTTreeAnalysis_cfi')
process.minimumBiasTTreeAnalysis.EBeam = config.comEnergy/2.
process.load('Utilities.AnalysisTools.trackHistos_cfi')
process.trackHistos.src = 'selectGoodTracks'

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
                        {'EnergyThresholdHB':1.0,'EnergyThresholdHE':1.5},
                        {'EnergyThresholdHB':2.0,'EnergyThresholdHE':2.5}]

attributes = attributesEnergyScale
attributes.extend(attributesThresholds)

from Utilities.PyConfigTools.analysisTools import *

makeAnalysis(process,'minimumBiasTTreeAnalysis','eventSelectionBscMinBiasOR')
makeAnalysis(process,'trackHistos','eventSelectionBscMinBiasOR')

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
    process.eventSelectionBscMinBiasORHFVetoPlus = cms.Sequence(process.eventSelectionBscMinBiasOR+process.hcalVetoHFPlus)
    process.eventSelectionBscMinBiasORHFVetoMinus = cms.Sequence(process.eventSelectionBscMinBiasOR+process.hcalVetoHFMinus)
    process.eventSelectionBscMinBiasORHEHFVetoPlus = cms.Sequence(process.eventSelectionBscMinBiasOR+process.hcalVetoHEHFPlus)
    process.eventSelectionBscMinBiasORHEHFVetoMinus = cms.Sequence(process.eventSelectionBscMinBiasOR+process.hcalVetoHEHFMinus)
    makeAnalysis(process,'minimumBiasTTreeAnalysis','eventSelectionBscMinBiasORHFVetoPlus')
    makeAnalysis(process,'minimumBiasTTreeAnalysis','eventSelectionBscMinBiasORHFVetoMinus')
    makeAnalysis(process,'minimumBiasTTreeAnalysis','eventSelectionBscMinBiasORHEHFVetoPlus')
    makeAnalysis(process,'minimumBiasTTreeAnalysis','eventSelectionBscMinBiasORHEHFVetoMinus')
    makeAnalysis(process,'trackHistos','eventSelectionBscMinBiasORHFVetoPlus')
    makeAnalysis(process,'trackHistos','eventSelectionBscMinBiasORHFVetoMinus')
    makeAnalysis(process,'trackHistos','eventSelectionBscMinBiasORHEHFVetoPlus')
    makeAnalysis(process,'trackHistos','eventSelectionBscMinBiasORHEHFVetoMinus')
    
process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("analysisMinBias_TTree_MinimumBias.root")
)
