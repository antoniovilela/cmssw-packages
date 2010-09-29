import FWCore.ParameterSet.Config as cms

# Settings
class config: pass
config.verbose = True
config.runOnMC = False
config.globalTagNameData = 'GR_R_36X_V12A::All'
config.globalTagNameMC = 'START36_V10::All'
config.outputTTreeFile = 'analysisTracks_MinimumBias.root'
config.instLumiROOTFile = 'lumibylsXing_132440-144114_7TeV_StreamExpress_Collisions10_V2_sub_132440.root'
config.comEnergy = 7000.0
config.trackAnalyzerName = 'trackHistoAnalyzer'
#config.trackTagName = 'selectGoodTracks'
config.trackTagName = 'analysisTracks'
config.runHCALFilter = True

process = cms.Process("Analysis")

process.load('FWCore.MessageService.MessageLogger_cfi')
process.MessageLogger.cerr.threshold = 'INFO'

process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(3000) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        'file:/data1/antoniov/MinimumBias_Commissioning10_GOODCOLL-Jun14thSkim_v1_RECO/MinimumBias_Commissioning10_GOODCOLL-Jun14thSkim_v1_RECO_EC45524A-E682-DF11-B8A7-001A92810AAA.root'
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

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string(config.outputTTreeFile)
)

###################################################################################
# HCAL reflagging
# HF RecHit reflagger -- specify type of HF cleaning to use
from Utilities.AnalysisTools.addHcalReflagging import addHFReflagging,addHBHEReflagging
# Adds hfrecoReflagged
process = addHFReflagging(process,version=10,isMC=config.runOnMC)
# HBHE RecHit reflagger
# Adds hbherecoReflagged
process = addHBHEReflagging(process)

if config.verbose:
    print "These are the severity levels for the various rechit flags:"
    print "(Severity > 10 causes rechit to be ignored by CaloTower maker)"
    for i in process.hcalRecAlgos.SeverityLevels: print i

###################################################################################

process.load('Utilities.AnalysisTools.lumiWeight_cfi')
process.lumiWeight.rootFileName = cms.string(config.instLumiROOTFile)

from Utilities.AnalysisTools.countsAnalyzer_cfi import countsAnalyzer
#countsAnalyzer.weightTag = cms.InputTag("lumiWeight")

process.xiTower.comEnergy = config.comEnergy
process.xiFromCaloTowers.comEnergy = config.comEnergy
process.xiFromJets.comEnergy = config.comEnergy
process.recoSequence = cms.Sequence(process.tracks*process.edmDump)
process.eventWeightSequence = cms.Sequence(process.lumiWeight)
# Reflagging and re-reco
process.reflagging_step = cms.Path(process.hfrecoReflagged+process.hbherecoReflagged)
process.rereco_step = cms.Path(process.caloTowersRec
                               *(process.recoJets*process.recoJetIds+process.recoTrackJets)
                               *process.recoJetAssociations
                               *process.metreco
                               ) # re-reco jets and met

process.selection_step = cms.Path(process.eventSelectionBscMinBiasOR)
process.eventWeight_step = cms.Path(process.eventWeightSequence)
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

###################################################################################

# Analysis modules
process.load('Utilities.AnalysisTools.trackHistos_cfi')
process.trackHistos.src = config.trackTagName
process.load('Utilities.AnalysisTools.trackHistoAnalyzer_cfi')
process.trackHistoAnalyzer.TrackTag = config.trackTagName

from Utilities.PyConfigTools.analysisTools import *

makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasOR')

if config.runHCALFilter:
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
