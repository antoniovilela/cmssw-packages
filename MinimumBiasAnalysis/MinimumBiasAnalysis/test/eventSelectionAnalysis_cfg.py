import FWCore.ParameterSet.Config as cms

# Settings
class config: pass
config.verbose = True
config.runOnMC = True
config.globalTagNameData = 'GR_R_36X_V12A::All'
config.globalTagNameMC = 'START36_V10::All'
config.outputTTreeFile = 'eventSelectionAnalysis_TTree_MinimumBias.root'
config.comEnergy = 7000.0
config.trackAnalyzerName = 'trackHistoAnalyzer'
config.trackTagName = 'analysisTracks'
config.generator = 'Pythia6'

process = cms.Process("Analysis")

process.load('FWCore.MessageService.MessageLogger_cfi')
process.MessageLogger.cerr.threshold = 'INFO'

process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(3000) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        'file:/data1/antoniov/MinBias_TuneD6T_7TeV-pythia6_START36_V10_SP10-v1_GEN-SIM-RECODEBUG/MinBias_TuneD6T_7TeV-pythia6_START36_V10_SP10-v1_GEN-SIM-RECODEBUG_F63DF090-6879-DF11-9E7D-0030487CDA68.root'
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

from Utilities.AnalysisTools.countsAnalyzer_cfi import countsAnalyzer

process.xiTower.comEnergy = config.comEnergy
process.xiFromCaloTowers.comEnergy = config.comEnergy
process.xiFromJets.comEnergy = config.comEnergy
process.recoSequence = cms.Sequence(process.tracks*process.edmDump)
# Reflagging and re-reco
process.reflagging_step = cms.Path(process.hfrecoReflagged+process.hbherecoReflagged)
process.rereco_step = cms.Path(process.caloTowersRec
                               *(process.recoJets*process.recoJetIds+process.recoTrackJets)
                               *process.recoJetAssociations
                               *process.metreco
                               ) # re-reco jets and met

process.reco_step = cms.Path(process.recoSequence)
if config.runOnMC:
    process.load('MinimumBiasAnalysis.MinimumBiasAnalysis.genChargedParticles_cfi')
    process.gen_step = cms.Path(process.genChargedParticles)

# Analysis modules
from MinimumBiasAnalysis.MinimumBiasAnalysis.minimumBiasTTreeAnalysis_cfi import minimumBiasTTreeAnalysis
minimumBiasTTreeAnalysis.EBeam = config.comEnergy/2.
minimumBiasTTreeAnalysis.TrackTag = config.trackTagName
process.minimumBiasTTreeAnalysisAll = minimumBiasTTreeAnalysis.clone()
process.minimumBiasTTreeAnalysisL1CollBscOr = minimumBiasTTreeAnalysis.clone()
process.minimumBiasTTreeAnalysishltBscMinBiasORBptxPlusORMinus = minimumBiasTTreeAnalysis.clone()
process.minimumBiasTTreeAnalysisVertexFilter = minimumBiasTTreeAnalysis.clone()
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
process.countsAll = countsAnalyzer.clone()
process.countsL1CollBscOr = countsAnalyzer.clone()
process.countshltBscMinBiasORBptxPlusORMinus = countsAnalyzer.clone()
process.countsVertexFilter = countsAnalyzer.clone()
process.countsFilterScraping = countsAnalyzer.clone()
process.countsHBHENoiseFilter = countsAnalyzer.clone()
process.countEvents_step = cms.Path(process.countsAll +
                                        process.minimumBiasTTreeAnalysisAll +
                                    process.l1CollBscOr +
                                    process.countsL1CollBscOr +
                                        process.minimumBiasTTreeAnalysisL1CollBscOr + 
                                    process.hltBscMinBiasORBptxPlusORMinusFilter + 
                                    process.countshltBscMinBiasORBptxPlusORMinus + 
                                        process.minimumBiasTTreeAnalysishltBscMinBiasORBptxPlusORMinus +
                                    process.primaryVertexFilter +
                                    process.countsVertexFilter + 
                                        process.minimumBiasTTreeAnalysisVertexFilter +
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
                                  TriggerResultsTag = cms.InputTag("TriggerResults::REDIGI36")) 
