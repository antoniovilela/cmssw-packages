import FWCore.ParameterSet.Config as cms

from Utilities.PyConfigTools.parseInput import parseInput
inputFields = ('runOnMC','generator','hltProcessNameMC')
requiredFields = ('runOnMC',)
inputOptions = parseInput(inputFields,requiredFields)
if not hasattr(inputOptions,'generator'): inputOptions.generator = 'Pythia8'
if not hasattr(inputOptions,'hltProcessNameMC'): inputOptions.hltProcessNameMC = 'REDIGI38X'

# Settings
class config: pass
config.runOnMC = inputOptions.runOnMC
config.generator = inputOptions.generator

config.verbose = True
config.globalTagNameData = 'GR_R_38X_V13A::All'
config.globalTagNameMC = 'START38_V12::All'
config.outputTTreeFile = 'eventSelectionAnalysis_TTree_MinimumBias.root'
config.comEnergy = 7000.0
config.trackAnalyzerName = 'trackHistoAnalyzer'
config.trackTagName = 'analysisTracks'
config.triggerResultsProcessNameMC = inputOptions.hltProcessNameMC
config.instLumiROOTFile = 'lumibylsXing_132440-144114_7TeV_Sep17ReReco_Collisions10_JSON_v2_sub_132440.root'

if config.runOnMC:
    if config.generator == 'Pythia8': config.fileNames = ['file:/storage2/antoniov/data1/MinBias_7TeV-pythia8_Fall10-START38_V12-v1_GEN-SIM-RECO/MinBias_7TeV-pythia8_Fall10-START38_V12-v1_GEN-SIM-RECO_A08BF063-13CC-DF11-8C8A-0030487D7EB1.root']
    elif config.generator == 'Pythia6': config.fileNames = ['file:/storage2/antoniov/data1/']
else: config.fileNames = ['file:/storage2/antoniov/data1/MinimumBias_Commissioning10-Sep17ReReco_v2_RECO/Run132599/MinimumBias_Commissioning10-Sep17ReReco_v2_RECO_80A3A8E6-46CA-DF11-AE95-00215E21D61E.root']

process = cms.Process("Analysis")

process.load('FWCore.MessageService.MessageLogger_cfi')
process.MessageLogger.cerr.threshold = 'DEBUG'
process.MessageLogger.debugModules = cms.untracked.vstring('minimumBiasTTreeAnalysishltBscMinBiasORBptxPlusORMinus')
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
"""
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
"""
###################################################################################

if not config.runOnMC:
    process.load('Utilities.AnalysisTools.lumiWeight_cfi')
    process.lumiWeight.rootFileName = cms.string(config.instLumiROOTFile)

#process.xiTower.comEnergy = config.comEnergy
#process.xiFromCaloTowers.comEnergy = config.comEnergy
#process.xiFromJets.comEnergy = config.comEnergy
process.recoSequence = cms.Sequence(process.tracks*process.pfCandidates*process.edmDump)
if not config.runOnMC: process.eventWeightSequence = cms.Sequence(process.lumiWeight)
# Reflagging and re-reco
"""
process.reflagging_step = cms.Path(process.hfrecoReflagged+process.hbherecoReflagged)
process.rereco_step = cms.Path(process.caloTowersRec
                               *(process.recoJets*process.recoJetIds+process.recoTrackJets)
                               *process.recoJetAssociations
                               *process.metreco
                               ) # re-reco jets and met
"""
if not config.runOnMC: process.eventWeight_step = cms.Path(process.eventWeightSequence)
process.reco_step = cms.Path(process.recoSequence)
if config.runOnMC:
    process.gen_step = cms.Path(process.genChargedParticles+
                                process.genProtonDissociative*process.edmNtupleMxGen+
                                process.genStableParticles*
                                process.etaMaxGen+process.etaMinGen*
                                process.edmNtupleEtaMaxGen+process.edmNtupleEtaMinGen)

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
process.minimumBiasTTreeAnalysisHcalNoiseFilter = minimumBiasTTreeAnalysis.clone()
process.minimumBiasTTreeAnalysisEtaMaxFilter = minimumBiasTTreeAnalysis.clone()

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
process.countsHcalNoiseFilter = countsAnalyzer.clone()
process.countsEtaMaxFilter = countsAnalyzer.clone()

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
                                    process.HBHENoiseFilter+process.hcalNoiseFilter +
                                        process.countsHcalNoiseFilter +
                                        process.minimumBiasTTreeAnalysisHcalNoiseFilter +
                                    process.etaMaxFilter +
                                        process.countsEtaMaxFilter +
                                        process.minimumBiasTTreeAnalysisEtaMaxFilter)  

process.countEvents_step.replace(process.primaryVertexFilter,process.primaryVertexFilterLooseNDOF0)

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
                                  HLTPath = "HLT_L1Tech_BSC_minBias_OR",
                                  TriggerResultsTag = cms.InputTag("TriggerResults","",config.triggerResultsProcessNameMC)) 
