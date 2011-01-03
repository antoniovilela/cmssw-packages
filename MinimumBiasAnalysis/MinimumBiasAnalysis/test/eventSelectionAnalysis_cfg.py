import FWCore.ParameterSet.Config as cms

from Utilities.PyConfigTools.parseInput import parseInput
inputFields = ('runOnMC','generator','hltProcessNameMC')
requiredFields = ('runOnMC',)
inputOptions = parseInput(inputFields,requiredFields)
if not hasattr(inputOptions,'generator'): inputOptions.generator = 'Pythia6'
if not hasattr(inputOptions,'hltProcessNameMC'): inputOptions.hltProcessNameMC = 'REDIGI36'

# Settings
class config: pass
config.runOnMC = inputOptions.runOnMC
config.generator = inputOptions.generator

config.verbose = True
config.globalTagNameData = 'GR_R_36X_V12A::All'
config.globalTagNameMC = 'START36_V10::All'
config.outputTTreeFile = 'eventSelectionAnalysis_TTree_MinimumBias.root'
config.comEnergy = 7000.0
config.trackAnalyzerName = 'trackHistoAnalyzer'
config.trackTagName = 'analysisTracks'
config.triggerResultsProcessNameMC = inputOptions.hltProcessNameMC
config.instLumiROOTFile = 'lumibylsXing_132440-137028_June14thReReco_Collisions10_JSON_v2_sub_132440.root'

if config.runOnMC: config.fileNames = ['file:/data1/antoniov/MinBias_TuneD6T_7TeV-pythia6_START36_V10_SP10-v1_GEN-SIM-RECODEBUG/MinBias_TuneD6T_7TeV-pythia6_START36_V10_SP10-v1_GEN-SIM-RECODEBUG_F63DF090-6879-DF11-9E7D-0030487CDA68.root']
else: config.fileNames = ['file:/data1/antoniov/MinimumBias_Commissioning10_GOODCOLL-Jun14thSkim_v1_RECO/MinimumBias_Commissioning10_GOODCOLL-Jun14thSkim_v1_RECO_EC45524A-E682-DF11-B8A7-001A92810AAA.root']

process = cms.Process("Analysis")

process.load('FWCore.MessageService.MessageLogger_cfi')
process.MessageLogger.cerr.threshold = 'DEBUG'
process.MessageLogger.debugModules = cms.untracked.vstring('minimumBiasTTreeAnalysisVertexFilter')
process.MessageLogger.categories.append('Analysis')
process.MessageLogger.cerr.DEBUG = cms.untracked.PSet(limit = cms.untracked.int32(0))
process.MessageLogger.cerr.Analysis = cms.untracked.PSet(limit = cms.untracked.int32(-1)) 

process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(3000) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring()
)
process.source.fileNames = config.fileNames

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

#process.xiTower.comEnergy = config.comEnergy
#process.xiFromCaloTowers.comEnergy = config.comEnergy
#process.xiFromJets.comEnergy = config.comEnergy
process.recoSequence = cms.Sequence(process.tracks*process.edmDump)
if not config.runOnMC: process.eventWeightSequence = cms.Sequence(process.lumiWeight)
# Reflagging and re-reco
process.reflagging_step = cms.Path(process.hfrecoReflagged+process.hbherecoReflagged)
process.rereco_step = cms.Path(process.caloTowersRec
                               *(process.recoJets*process.recoJetIds+process.recoTrackJets)
                               *process.recoJetAssociations
                               *process.metreco
                               ) # re-reco jets and met
if not config.runOnMC: process.eventWeight_step = cms.Path(process.eventWeightSequence)
process.reco_step = cms.Path(process.recoSequence)
if config.runOnMC:
    process.load('MinimumBiasAnalysis.MinimumBiasAnalysis.genChargedParticles_cfi')
    process.gen_step = cms.Path(process.genChargedParticles)

# Analysis modules
from MinimumBiasAnalysis.MinimumBiasAnalysis.minimumBiasTTreeAnalysis_cfi import minimumBiasTTreeAnalysis
minimumBiasTTreeAnalysis.EBeam = config.comEnergy/2.
minimumBiasTTreeAnalysis.TrackTag = config.trackTagName
process.minimumBiasTTreeAnalysisAll = minimumBiasTTreeAnalysis.clone()
process.minimumBiasTTreeAnalysishltBscMinBiasORBptxPlusORMinus = minimumBiasTTreeAnalysis.clone()
process.minimumBiasTTreeAnalysisBPTX = minimumBiasTTreeAnalysis.clone()
process.minimumBiasTTreeAnalysisBscOr = minimumBiasTTreeAnalysis.clone()
process.minimumBiasTTreeAnalysisVertexFilter = minimumBiasTTreeAnalysis.clone()
process.minimumBiasTTreeAnalysisBeamHaloVeto = minimumBiasTTreeAnalysis.clone()
process.minimumBiasTTreeAnalysisFilterScraping = minimumBiasTTreeAnalysis.clone()
process.minimumBiasTTreeAnalysisHBHENoiseFilter = minimumBiasTTreeAnalysis.clone()

process.load('Utilities.AnalysisTools.trackHistos_cfi')
process.trackHistos.src = config.trackTagName
process.load('Utilities.AnalysisTools.trackHistoAnalyzer_cfi')
process.trackHistoAnalyzer.TrackTag = config.trackTagName

processIdFilters = None
if config.runOnMC:
    from MinimumBiasAnalysis.MinimumBiasAnalysis.addProcessIdFilters import addProcessIdFilters
    if config.verbose:
        print 'Using generator type',config.generator
    processIdFilters = addProcessIdFilters(process,config.generator)

#process.analysisPreSelection = cms.Sequence(process.processIdPythia6_SD)

# Paths for event counting
from Utilities.AnalysisTools.countsAnalyzer_cfi import countsAnalyzer  
process.countsAll = countsAnalyzer.clone()
process.countshltBscMinBiasORBptxPlusORMinus = countsAnalyzer.clone()
process.countsBPTX = countsAnalyzer.clone()
process.countsBscOr = countsAnalyzer.clone()
process.countsVertexFilter = countsAnalyzer.clone()
process.countsBeamHaloVeto = countsAnalyzer.clone()
process.countsFilterScraping = countsAnalyzer.clone()
process.countsHBHENoiseFilter = countsAnalyzer.clone()

process.countEvents_step = cms.Path(    process.countsAll +
                                        #process.minimumBiasTTreeAnalysisAll +
                                    process.hltBscMinBiasORBptxPlusORMinusFilter +
                                        process.countshltBscMinBiasORBptxPlusORMinus +
                                        process.minimumBiasTTreeAnalysishltBscMinBiasORBptxPlusORMinus +
                                    process.bptx +
                                        process.countsBPTX +
                                        process.minimumBiasTTreeAnalysisBPTX + 
                                    process.bscOr + 
                                        process.countsBscOr +
                                        process.minimumBiasTTreeAnalysisBscOr +
                                    process.primaryVertexFilter +
                                        process.countsVertexFilter + 
                                        process.minimumBiasTTreeAnalysisVertexFilter +
                                    process.beamHaloVeto + 
                                        process.countsBeamHaloVeto +
                                        process.minimumBiasTTreeAnalysisBeamHaloVeto +
                                    process.filterScraping +
                                        process.countsFilterScraping + 
                                        process.minimumBiasTTreeAnalysisFilterScraping +
                                    process.HBHENoiseFilter +
                                        process.countsHBHENoiseFilter +
                                        process.minimumBiasTTreeAnalysisHBHENoiseFilter)  

###################################################################################

# Add MC-specific paths
# ...

if config.runOnMC:
    from Utilities.PyConfigTools.removeFromPaths import removeFromPaths
    removeFromPaths(process,'bptx')
    removeFromPaths(process,'hltBscMinBiasORBptxPlusORMinusFilter')

    from Utilities.PyConfigTools.setAnalyzerAttributes import setAnalyzerAttributes
    setAnalyzerAttributes(process,'minimumBiasTTreeAnalysis',
                                  AccessMCInfo = True, 
                                  TriggerResultsTag = cms.InputTag("TriggerResults::%s" % config.triggerResultsProcessNameMC)) 
