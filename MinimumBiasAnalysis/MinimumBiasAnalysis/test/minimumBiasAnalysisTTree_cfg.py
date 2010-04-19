import FWCore.ParameterSet.Config as cms

# Settings
class config: pass
#config.comEnergy = 900.0
config.comEnergy = 2360.0
config.varyAttributes = True
config.runBSCAND = False
config.runPixel = False
config.runOfflineOnly = True
config.runNoBPTX = False
config.runNoColl = False
config.runBPTX = False

# Skim sequences
from minimumBiasAnalysis_cfg import process
if hasattr(process,'output'): del process.output
if hasattr(process,'out_step'): del process.out_step
process.MessageLogger.cerr.threshold = 'WARNING'
process.maxEvents.input = 20000
process.xiTower.comEnergy = config.comEnergy
process.xiFromCaloTowers.comEnergy = config.comEnergy
process.xiFromJets.comEnergy = config.comEnergy

# 900 GeV
#from fileNames_MinimumBias_Jan29ReReco_124023_Lumi_41_96 import fileNames
#process.source.fileNames = fileNames
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('124030:2-124030:9999','124027:24-124027:9999','124025:5-124025:13','124024:2-124024:83','124023:38-124023:9999','124022:66-124022:179','124020:12-124020:94','124009:1-124009:68')
# 2360 GeV
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('124120:1-124120:9999')

process.load('MinimumBiasAnalysis.MinimumBiasAnalysis.minimumBiasTTreeAnalysis_cfi')
process.minimumBiasTTreeAnalysis.EBeam = config.comEnergy/2

attributesEnergyScale = [{'ApplyEnergyScaleHCAL':False},
                         {'ApplyEnergyScaleHCAL':True,'EnergyScaleFactorHCAL':0.9,'HFTowerSummaryTag':'hfTowerScale09'},
                         {'ApplyEnergyScaleHCAL':True,'EnergyScaleFactorHCAL':1.1,'HFTowerSummaryTag':'hfTowerScale11'}]

attributesThresholds = [{'HFThresholdIndex':18,'EnergyThresholdHBHE':3.0,'EnergyThresholdHF':3.6},
                        {'HFThresholdIndex':30,'EnergyThresholdHBHE':3.0,'EnergyThresholdHF':6.0},
                        {'HFThresholdIndex':40,'EnergyThresholdHBHE':3.0,'EnergyThresholdHF':8.0},
                        {'HFThresholdIndex':20,'EnergyThresholdHBHE':2.0,'EnergyThresholdHF':4.0}]

attributes = attributesEnergyScale
attributes.extend(attributesThresholds)

from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.analysisTools import *
if config.varyAttributes:
    makeAnalysis(process,'minimumBiasTTreeAnalysis','eventSelectionMinBiasBSCOR',attributes)
    if config.runBSCAND: makeAnalysis(process,'minimumBiasTTreeAnalysis','eventSelectionMinBiasBSCAND',attributes)
    if config.runPixel: makeAnalysis(process,'minimumBiasTTreeAnalysis','eventSelectionMinBiasPixel',attributes)
else:
    makeAnalysis(process,'minimumBiasTTreeAnalysis','eventSelectionMinBiasBSCOR')
    if config.runBSCAND: makeAnalysis(process,'minimumBiasTTreeAnalysis','eventSelectionMinBiasBSCAND')
    if config.runPixel: makeAnalysis(process,'minimumBiasTTreeAnalysis','eventSelectionMinBiasPixel')

if config.runOfflineOnly:
    makeAnalysis(process,'minimumBiasTTreeAnalysis','eventSelection')

if config.runBPTX:
    #makeAnalysis(process,'minimumBiasTTreeAnalysis','eventSelectionBPTX')
    makeAnalysis(process,'minimumBiasTTreeAnalysis','eventSelectionL1Tech4')

if config.runNoBPTX:
    makeAnalysis(process,'minimumBiasTTreeAnalysis','eventSelectionMinBiasBSCORNoBPTX')
    #makeAnalysis(process,'minimumBiasTTreeAnalysis','eventSelectionMinBiasPixelNoBPTX')

if config.runNoColl:
    makeAnalysis(process,'minimumBiasTTreeAnalysis','eventSelectionMinBiasBSCORNoColl')
    #makeAnalysis(process,'minimumBiasTTreeAnalysis','eventSelectionMinBiasPixelNoColl')

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("analysisMinBias_TTree_MinimumBias.root")
)
