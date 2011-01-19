import FWCore.ParameterSet.Config as cms

def getBinBoundaries(min,max,binSize,nDivisions=4):
    #nDivisions = 5
    binList = []
    binList.append(min)
    while binList[-1] < max:
        division = int((binList[-1] - min)*nDivisions/(max - min))
        factor = 2**division
        binList.append(binList[-1] + factor*binSize)

    if binList[-1] > max:
        binList = binList[:-2]
        binList.append(max)

    return binList

from Utilities.PyConfigTools.parseInput import parseInput
inputFields = ('runOnMC','generator')
inputOptions = parseInput(inputFields) 

# Settings
class config: pass
#config.fileNames = ['file:/data1/antoniov/MinimumBias_Commissioning10_GOODCOLL-Jun14thSkim_v1_RECO/MinimumBias_Commissioning10_GOODCOLL-Jun14thSkim_v1_RECO_EC45524A-E682-DF11-B8A7-001A92810AAA.root']
config.fileNames = ['file:/storage1/antoniov/data1/MinBias_7TeV-pythia8_START36_V10_SP10-v1_GEN-SIM-RECODEBUG/MinBias_7TeV-pythia8_START36_V10_SP10-v1_GEN-SIM-RECODEBUG_F2E56105-8E74-DF11-9C73-00237DA1CDBE.root']
config.verbose = True
if not hasattr(inputOptions,'runOnMC'): config.runOnMC = False
else: config.runOnMC = inputOptions.runOnMC
if not hasattr(inputOptions,'generator'): config.generator = 'Pythia6'
else: config.generator = inputOptions.generator
config.globalTagNameData = 'GR_R_36X_V12A::All'
config.globalTagNameMC = 'START36_V10::All'
if config.runOnMC: config.outputTTreeFile = 'analysisTracks_MinBias.root'
else: config.outputTTreeFile = 'analysisTracks_MinimumBias.root'
config.instLumiROOTFile = 'lumibylsXing_132440-137028_June14thReReco_Collisions10_JSON_v2_sub_132440.root'
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
    fileNames = cms.untracked.vstring(config.fileNames)
)
# Import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.GeometryExtended_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.Reconstruction_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
if config.runOnMC: process.GlobalTag.globaltag = config.globalTagNameMC
else: process.GlobalTag.globaltag = config.globalTagNameData

#process.load("MinimumBiasAnalysis.MinimumBiasAnalysis.analysisSequences_cff")
process.load("Utilities.AnalysisSequences.minimumBiasAnalysisSequences_cff")

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

"""
if not config.runOnMC:
    process.load('Utilities.AnalysisTools.lumiWeight_cfi')
    process.lumiWeight.rootFileName = cms.string(config.instLumiROOTFile)
"""

from Utilities.AnalysisTools.countsAnalyzer_cfi import countsAnalyzer
#if not config.runOnMC: countsAnalyzer.weightTag = cms.InputTag("lumiWeight")

"""
process.xiTower.comEnergy = config.comEnergy
process.xiFromCaloTowers.comEnergy = config.comEnergy
process.xiFromJets.comEnergy = config.comEnergy
"""
process.recoSequence = cms.Sequence(process.tracks)
#if not config.runOnMC: process.eventWeightSequence = cms.Sequence(process.lumiWeight)
# Reflagging and re-reco
process.reflagging_step = cms.Path(process.hfrecoReflagged+process.hbherecoReflagged)
process.rereco_step = cms.Path(process.caloTowersRec
                               *(process.recoJets*process.recoJetIds+process.recoTrackJets)
                               *process.recoJetAssociations
                               *process.metreco
                               ) # re-reco jets and met

process.selection_step = cms.Path(process.eventSelectionBscMinBiasOR)
#if not config.runOnMC: process.eventWeight_step = cms.Path(process.eventWeightSequence)
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
maxPt = process.trackHistoAnalyzer.MaxPt.value()
minPt = process.trackHistoAnalyzer.MinPt.value()
nBinsPt = process.trackHistoAnalyzer.NBinsPt.value()
ptBins = getBinBoundaries(minPt,maxPt,(maxPt-minPt)/nBinsPt,nDivisions=5)
maxPtSum = process.trackHistoAnalyzer.MaxPtSum.value()
minPtSum = process.trackHistoAnalyzer.MinPtSum.value()
nBinsPtSum = process.trackHistoAnalyzer.NBinsPtSum.value()
ptSumBins = getBinBoundaries(minPtSum,maxPtSum,(maxPtSum-minPtSum)/nBinsPtSum,nDivisions=4)
maxNTracks = process.trackHistoAnalyzer.MaxNTracks.value()
minNTracks = process.trackHistoAnalyzer.MinNTracks.value()
nBinsNTracks = process.trackHistoAnalyzer.NBinsNTracks.value()
nTracksBins = getBinBoundaries(minNTracks,maxNTracks,(maxNTracks-minNTracks)/nBinsNTracks,nDivisions=2)
process.trackHistoAnalyzer.VarBin = cms.PSet(
    pt = cms.vdouble(ptBins),
    ptSum = cms.vdouble(ptSumBins),
    nTracks = cms.vdouble(nTracksBins)
)

from Utilities.PyConfigTools.analysisTools import *

processIdFilters = None
if config.runOnMC:
    from MinimumBiasAnalysis.MinimumBiasAnalysis.addProcessIdFilters import addProcessIdFilters
    if config.verbose:
        print 'Using generator type',config.generator
    processIdFilters = addProcessIdFilters(process,config.generator)

# Analysis modules
makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasOR')
if config.runOnMC:
    makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasOR',filters=processIdFilters)
  
if config.runHCALFilter:
    makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasORHFVetoPlus')
    makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasORHFVetoMinus')
    makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasORHEHFVetoPlus')
    makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasORHEHFVetoMinus')
    makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasOREEHEHFVetoPlus')
    makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasOREEHEHFVetoMinus')
    makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasORHFPlusAndMinus')
    makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasORHEHFPlusAndMinus')
    makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasOREEHEHFPlusAndMinus')
    makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasORSumEMaxHFPlus4')
    makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasORSumEMaxHFPlus8')
    makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasORSumEMaxHFPlus12')
    makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasORSumEMaxHFPlus16')
    makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasORSumEMaxHFMinus4')
    makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasORSumEMaxHFMinus8')
    makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasORSumEMaxHFMinus12')
    makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasORSumEMaxHFMinus16')
    """
    makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasORSumETMaxHFPlus05')
    makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasORSumETMaxHFPlus10')
    makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasORSumETMaxHFPlus15')
    makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasORSumETMaxHFPlus20')
    makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasORSumETMaxHFMinus05')
    makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasORSumETMaxHFMinus10')
    makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasORSumETMaxHFMinus15')
    makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasORSumETMaxHFMinus20')
    """
    if config.runOnMC:
        makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasORHFVetoPlus',filters=processIdFilters)
        makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasORHFVetoMinus',filters=processIdFilters)
        makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasORHEHFVetoPlus',filters=processIdFilters)
        makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasORHEHFVetoMinus',filters=processIdFilters)
        makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasOREEHEHFVetoPlus',filters=processIdFilters)
        makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasOREEHEHFVetoMinus',filters=processIdFilters)
        makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasORHFPlusAndMinus',filters=processIdFilters)
        makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasORHEHFPlusAndMinus',filters=processIdFilters)
        makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasOREEHEHFPlusAndMinus',filters=processIdFilters)
        makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasORSumEMaxHFPlus4',filters=processIdFilters)
        makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasORSumEMaxHFPlus8',filters=processIdFilters)
        makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasORSumEMaxHFPlus12',filters=processIdFilters)
        makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasORSumEMaxHFPlus16',filters=processIdFilters)
        makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasORSumEMaxHFMinus4',filters=processIdFilters)
        makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasORSumEMaxHFMinus8',filters=processIdFilters)
        makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasORSumEMaxHFMinus12',filters=processIdFilters)
        makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasORSumEMaxHFMinus16',filters=processIdFilters)
        """
        makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasORSumETMaxHFPlus05',filters=processIdFilters)
        makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasORSumETMaxHFPlus10',filters=processIdFilters)
        makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasORSumETMaxHFPlus15',filters=processIdFilters)
        makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasORSumETMaxHFPlus20',filters=processIdFilters)
        makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasORSumETMaxHFMinus05',filters=processIdFilters)
        makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasORSumETMaxHFMinus10',filters=processIdFilters)
        makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasORSumETMaxHFMinus15',filters=processIdFilters)
        makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasORSumETMaxHFMinus20',filters=processIdFilters)
        """
    
if config.runOnMC:
    from Utilities.PyConfigTools.removeFromPaths import removeFromPaths
    removeFromPaths(process,'bptx')
    removeFromPaths(process,'hltBscMinBiasORBptxPlusORMinusFilter')
   
