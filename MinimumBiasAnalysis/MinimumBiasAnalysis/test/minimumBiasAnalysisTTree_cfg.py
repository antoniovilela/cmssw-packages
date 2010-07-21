import FWCore.ParameterSet.Config as cms

# Settings
class config: pass
config.comEnergy = 7000.0
config.varyAttributes = True
config.runOfflineOnly = True
config.runNoColl = False
config.runBPTX = False

# Skim sequences
from minimumBiasAnalysis_cfg import process
if hasattr(process,'output'): del process.output
if hasattr(process,'out_step'): del process.out_step
process.MessageLogger.cerr.threshold = 'INFO'
process.maxEvents.input = 20000
process.GlobalTag.globaltag = 'GR_R_36X_V12A::All'
process.xiTower.comEnergy = config.comEnergy
process.xiFromCaloTowers.comEnergy = config.comEnergy
process.xiFromJets.comEnergy = config.comEnergy

process.load('MinimumBiasAnalysis.MinimumBiasAnalysis.minimumBiasTTreeAnalysis_cfi')
process.minimumBiasTTreeAnalysis.EBeam = config.comEnergy/2.

attributesEnergyScale = [{'ApplyEnergyScaleHCAL':False},
                         {'ApplyEnergyScaleHCAL':True,'EnergyScaleFactorHCAL':0.90,'HFTowerSummaryTag':'hfTowerScale090'},
                         {'ApplyEnergyScaleHCAL':True,'EnergyScaleFactorHCAL':0.92,'HFTowerSummaryTag':'hfTowerScale092'},
                         {'ApplyEnergyScaleHCAL':True,'EnergyScaleFactorHCAL':0.95,'HFTowerSummaryTag':'hfTowerScale095'},
                         {'ApplyEnergyScaleHCAL':True,'EnergyScaleFactorHCAL':0.98,'HFTowerSummaryTag':'hfTowerScale098'},  
                         {'ApplyEnergyScaleHCAL':True,'EnergyScaleFactorHCAL':1.02,'HFTowerSummaryTag':'hfTowerScale102'},
                         {'ApplyEnergyScaleHCAL':True,'EnergyScaleFactorHCAL':1.05,'HFTowerSummaryTag':'hfTowerScale105'},
                         {'ApplyEnergyScaleHCAL':True,'EnergyScaleFactorHCAL':1.08,'HFTowerSummaryTag':'hfTowerScale108'}, 
                         {'ApplyEnergyScaleHCAL':True,'EnergyScaleFactorHCAL':1.10,'HFTowerSummaryTag':'hfTowerScale110'}]

attributesThresholds = [{'HFThresholdIndex':18,'EnergyThresholdHBHE':3.0,'EnergyThresholdHF':3.6},
                        {'HFThresholdIndex':30,'EnergyThresholdHBHE':3.0,'EnergyThresholdHF':6.0},
                        {'HFThresholdIndex':40,'EnergyThresholdHBHE':3.0,'EnergyThresholdHF':8.0},
                        {'HFThresholdIndex':20,'EnergyThresholdHBHE':2.0,'EnergyThresholdHF':4.0}]

attributes = attributesEnergyScale
attributes.extend(attributesThresholds)

from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.analysisTools import *
if config.varyAttributes:
    makeAnalysis(process,'minimumBiasTTreeAnalysis','eventSelectionBscMinBiasOR',attributes)
else:
    makeAnalysis(process,'minimumBiasTTreeAnalysis','eventSelectionBscMinBiasOR')

if config.runOfflineOnly:
    makeAnalysis(process,'minimumBiasTTreeAnalysis','eventSelection')

if config.runBPTX:
    makeAnalysis(process,'minimumBiasTTreeAnalysis','eventSelectionBPTX')
    makeAnalysis(process,'minimumBiasTTreeAnalysis','eventSelectionL1Tech4')

if config.runNoColl:
    makeAnalysis(process,'minimumBiasTTreeAnalysis','eventSelectionBscMinBiasORNoColl')
    makeAnalysis(process,'minimumBiasTTreeAnalysis','eventSelectionBscMinBiasORBPTXOR')

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("analysisMinBias_TTree_MinimumBias.root")
)
