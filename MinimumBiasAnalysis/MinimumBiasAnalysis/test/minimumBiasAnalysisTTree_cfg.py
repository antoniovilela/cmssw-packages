import FWCore.ParameterSet.Config as cms

# Settings
class config: pass
config.verbose = True
config.writeEdmOutput = False
config.runOnMC = False
config.globalTagNameData = 'GR_R_36X_V12A::All'
config.globalTagNameMC = 'START36_V10::All'
config.outputEdmFile = '/tmp/antoniov/minimumBias.root'
config.outputTTreeFile = 'analysisMinBias_TTree_MinimumBias.root'
config.instLumiROOTFile = 'lumibylsXing_132440-137028_June14thReReco_Collisions10_JSON_v2_sub_132440.root'
config.comEnergy = 7000.0
config.trackAnalyzerName = 'trackHistoAnalyzer'
#config.trackTagName = 'selectGoodTracks'
config.trackTagName = 'analysisTracks'
config.generator = 'Pythia6'
config.varyAttributes = True
config.runOfflineOnly = True
config.runNoColl = True
config.runBPTX = True
config.runHCALFilter = True

process = cms.Process("Analysis")

process.load('FWCore.MessageService.MessageLogger_cfi')
process.MessageLogger.cerr.threshold = 'INFO'

process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(3000) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        #'file:/data1/antoniov/MinimumBias_Commissioning10_GOODCOLL-Jun14thSkim_v1_RECO/MinimumBias_Commissioning10_GOODCOLL-Jun14thSkim_v1_RECO_EC45524A-E682-DF11-B8A7-001A92810AAA.root'
        'file:/data1/antoniov/ZeroBias_Commissioning10-Jun14thReReco_v1_RECO/ZeroBias_Commissioning10-Jun14thReReco_v1_RECO_6AF419E4-E17B-DF11-AEB6-0026189437FC.root'
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
# Adds hfrecoReflagged
reflagVersion = None
if config.runOnMC: reflagVersion = 2
else: reflagVersion = 10
if config.verbose:
    print "Adding HF reflagging with version",reflagVersion
process = addHFReflagging(process,version=reflagVersion,isMC=config.runOnMC)
# HBHE RecHit reflagger
# Adds hbherecoReflagged
if config.verbose:
    print "Adding HB/HE reflagging"
process = addHBHEReflagging(process)

if config.verbose:
    print "These are the severity levels for the various rechit flags:"
    print "(Severity > 10 causes rechit to be ignored by CaloTower maker)"
    for i in process.hcalRecAlgos.SeverityLevels: print i

###################################################################################

if not config.runOnMC:
    process.load('Utilities.AnalysisTools.lumiWeight_cfi')
    process.lumiWeight.rootFileName = cms.string(config.instLumiROOTFile)

from Utilities.AnalysisTools.countsAnalyzer_cfi import countsAnalyzer

process.xiTower.comEnergy = config.comEnergy
process.xiFromCaloTowers.comEnergy = config.comEnergy
process.xiFromJets.comEnergy = config.comEnergy
process.recoSequence = cms.Sequence(process.tracks*process.edmDump)
if not config.runOnMC: process.eventWeightSequence = cms.Sequence(process.lumiWeight)
# Reflagging and re-reco
process.reflagging_step = cms.Path(process.hfrecoReflagged+process.hbherecoReflagged)
process.rereco_step = cms.Path(process.caloTowersRec
                               *(process.recoJets*process.recoJetIds+process.recoTrackJets)
                               *process.recoJetAssociations
                               *process.metreco
                               ) # re-reco jets and met

process.selection_step = cms.Path(process.eventSelectionBscMinBiasOR)
if not config.runOnMC: process.eventWeight_step = cms.Path(process.eventWeightSequence)
process.reco_step = cms.Path(process.eventSelection+process.recoSequence)
# Path for event counting  
process.countsAll = countsAnalyzer.clone()
process.countsL1CollBscOr = countsAnalyzer.clone()
process.countshltBscMinBiasORBptxPlusORMinus = countsAnalyzer.clone()
process.countsVertexFilter = countsAnalyzer.clone()
process.countsFilterScraping = countsAnalyzer.clone()
process.countsHBHENoiseFilter = countsAnalyzer.clone()
process.countEvents_step = cms.Path(process.countsAll +
                                    process.l1CollBscOr +
                                    process.countsL1CollBscOr +
                                    process.hltBscMinBiasORBptxPlusORMinusFilter +
                                    process.countshltBscMinBiasORBptxPlusORMinus +
                                    process.primaryVertexFilter +
                                    process.countsVertexFilter +  
                                    process.filterScraping +
                                    process.countsFilterScraping + 
                                    process.HBHENoiseFilter +
                                    process.countsHBHENoiseFilter)  

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

processIdFilters = None
if config.runOnMC:
    from MinimumBiasAnalysis.MinimumBiasAnalysis.addProcessIdFilters import addProcessIdFilters
    if config.verbose:
        print 'Using generator type',config.generator
    processIdFilters = addProcessIdFilters(process,config.generator)

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

# Add MC-specific paths
# ...

if config.runOnMC:
    from Utilities.PyConfigTools.removeFromPaths import removeFromPaths
    removeFromPaths(process,'bptx')
    removeFromPaths(process,'hltBscMinBiasORBptxPlusORMinusFilter')
