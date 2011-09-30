import FWCore.ParameterSet.Config as cms

# Settings
class config: pass
config.runOnMC = True
config.generator = 'Pythia8'
config.comEnergy = 7000.0
config.maxEvents = 3000
config.verbose = True
#---
config.writeEdmOutput = False
config.outputEdmFile = 'minimumBias.root'
config.outputTTreeFile = 'analysisMinBias_TTree_MinimumBias.root'
#---
config.globalTagNameData = 'GR_R_42_V19::All'
config.globalTagNameMC = 'START42_V13::All'
config.instLumiROOTFile = 'lumibylsXing_132440-144114_7TeV_Sep17ReReco_Collisions10_JSON_v2_sub_132440.root'
config.trackAnalyzerName = 'trackHistoAnalyzer'
config.trackTagName = 'analysisTracks'
config.vertexTagName = 'analysisVertices'
#---
config.switchPVFilter = True #primaryVertexFilterLooseNDOF0
config.varyAttributes = False
config.runPFlowThresholdAnalysis = True
config.runOfflineOnly = True
config.runNoColl = False
config.runBPTX = False
config.runHCALFilter = False
config.runEtaMaxFilter = True
config.runCastorFilter = True

if not config.runOnMC:
    config.fileNames = ['file:/storage2/antoniov/data1/MinimumBias_Commissioning10_May19ReReco-v1_RECO/MinimumBias_Commissioning10_May19ReReco-v1_RECO_0C0FA77A-0D83-E011-82D3-001A64787060.root']
else:
    config.fileNames = ['file:/storage2/antoniov/data1/Pythia8MBR-reco423patch3/step2_0.root',
                        'file:/storage2/antoniov/data1/Pythia8MBR-reco423patch3/step2_1.root',
                        'file:/storage2/antoniov/data1/Pythia8MBR-reco423patch3/step2_10.root']

if config.runOnMC: config.outputTTreeFile = 'analysisMinBias_TTree_MinBias.root'

process = cms.Process("Analysis")

process.load('FWCore.MessageService.MessageLogger_cfi')
process.MessageLogger.cerr.threshold = 'INFO'
"""
process.MessageLogger.debugModules = cms.untracked.vstring('minimumBiasTTreeAnalysis_eventSelectionBscMinBiasOR')
process.MessageLogger.destinations = cms.untracked.vstring('cerr')
process.MessageLogger.categories.append('Analysis')
process.MessageLogger.cerr.Analysis = cms.untracked.PSet(limit = cms.untracked.int32(-1))
"""

process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(config.maxEvents) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring()
)
process.source.fileNames = config.fileNames
if config.runOnMC: process.source.duplicateCheckMode = cms.untracked.string('noDuplicateCheck')

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

###################################################################################
# CASTOR RecHit Corrector
if not config.runOnMC:
    process.castorRecHitCorrector = cms.EDProducer("RecHitCorrector",
        rechitLabel = cms.InputTag("castorreco","","RECO"),
        revertFactor = cms.double(1),
        doInterCalib = cms.bool(False)
    )

    process.load("CondCore.DBCommon.CondDBSetup_cfi")
    process.CastorDbProducer = cms.ESProducer("CastorDbProducer")
    process.es_castor_pool = cms.ESSource("PoolDBESSource",
        process.CondDBSetup,
        timetype = cms.string('runnumber'),
        connect = cms.string('frontier://cmsfrontier.cern.ch:8000/FrontierProd/CMS_COND_31X_HCAL'),
        authenticationMethod = cms.untracked.uint32(0),
        toGet = cms.VPSet(
            cms.PSet(
                record = cms.string('CastorChannelQualityRcd'),  
                tag = cms.string('CastorChannelQuality_v2.2_offline')
            )
        )
    )
    process.es_prefer_castor = cms.ESPrefer('PoolDBESSource','es_castor_pool')

    #process.castorInvalidDataFilter = cms.EDFilter("CastorInvalidDataFilter")
    process.castorSequence = cms.Sequence(process.castorInvalidDataFilter+process.castorRecHitCorrector)

###################################################################################

if not config.runOnMC:
    process.load('Utilities.AnalysisTools.lumiWeight_cfi')
    process.lumiWeight.rootFileName = cms.string(config.instLumiROOTFile)

from Utilities.AnalysisTools.countsAnalyzer_cfi import countsAnalyzer

#process.recoSequence = cms.Sequence(process.tracks*process.pfCandidates*process.edmDump)
process.recoSequence = cms.Sequence(process.vertices*process.tracks*process.pfCandidates*process.edmDump)
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
if not config.runOnMC: process.castor_step = cms.Path(process.castorSequence)
if not config.runOnMC: process.eventWeight_step = cms.Path(process.eventWeightSequence)
process.reco_step = cms.Path(process.recoSequence)
if config.runOnMC:
    process.gen_step = cms.Path(process.genChargedParticles+
                                process.genProtonDissociative*process.edmNtupleMxGen+
                                process.genStableParticles*
                                process.etaMaxGen+process.etaMinGen*
                                process.edmNtupleEtaMaxGen+process.edmNtupleEtaMinGen)

process.selection_step = cms.Path(process.eventSelectionBscMinBiasOR)

# Path for event counting
process.countsAll = countsAnalyzer.clone()
process.countshltBscMinBiasORBptxPlusORMinus = countsAnalyzer.clone()
process.countsBPTX = countsAnalyzer.clone()
process.countsBscOr = countsAnalyzer.clone()
process.countsVertexFilter = countsAnalyzer.clone()
process.countsBeamHaloVeto = countsAnalyzer.clone()
process.countsFilterScraping = countsAnalyzer.clone()
process.countsHcalNoiseFilter = countsAnalyzer.clone()
process.countsMultipleVertexVeto = countsAnalyzer.clone()
process.countsEtaMinFilter = countsAnalyzer.clone()
process.countsCastorVeto = countsAnalyzer.clone()  
process.countEvents_step = cms.Path(process.countsAll +
                                    process.hltBscMinBiasORBptxPlusORMinusFilter +
                                    process.countshltBscMinBiasORBptxPlusORMinus +
                                    process.bptx +
                                    process.countsBPTX + 
                                    process.bscOr +
                                    process.countsBscOr +
                                    process.primaryVertexFilter +
                                    process.countsVertexFilter +
                                    process.beamHaloVeto +
                                    process.countsBeamHaloVeto +
                                    process.filterScraping +
                                    process.countsFilterScraping +
                                    process.HBHENoiseFilter+process.hcalNoiseFilter +
                                    process.countsHcalNoiseFilter +
                                    process.multipleVertexVeto + 
                                    process.countsMultipleVertexVeto +  
                                    process.etaMinFilter +
                                    process.countsEtaMinFilter +
                                    process.castorVeto +
                                    process.countsCastorVeto)

if config.writeEdmOutput: process.out_step = cms.EndPath(process.output)

###################################################################################

# Analysis modules
if config.runOnMC: process.castorActivityFilter.CastorRecHitTag = 'castorreco'

process.load('MinimumBiasAnalysis.MinimumBiasAnalysis.minimumBiasTTreeAnalysis_cfi')
process.minimumBiasTTreeAnalysis.EBeam = config.comEnergy/2.
process.minimumBiasTTreeAnalysis.TrackTag = config.trackTagName
process.minimumBiasTTreeAnalysis.VertexTag = config.vertexTagName
"""
process.minimumBiasTTreeAnalysisNoCleaning = process.minimumBiasTTreeAnalysis.clone(
    CaloTowerTag = cms.InputTag("towerMaker::RECO")
)
"""

process.load('Utilities.AnalysisTools.trackHistos_cfi')
process.trackHistos.src = config.trackTagName
process.load('Utilities.AnalysisTools.trackHistoAnalyzer_cfi')
process.trackHistoAnalyzer.TrackTag = config.trackTagName
process.load('Utilities.AnalysisTools.pfHistos_cfi')
process.pFlowHistos = process.pfHistos.clone( src = 'particleFlow' )
process.pFlowWithThresholdsHistos = process.pfHistos.clone( src = 'pfCandidateNoiseThresholds' )
process.load('Utilities.AnalysisTools.pFlowNoiseAnalyzer_cfi')
process.pFlowNoiseAnalyzer.particleFlowTag = 'particleFlow'

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
#makeAnalysis(process,'minimumBiasTTreeAnalysisNoCleaning','eventSelectionBscMinBiasOR')
makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasOR')
makeAnalysis(process,'pFlowHistos','eventSelectionBscMinBiasOR')
makeAnalysis(process,'pFlowWithThresholdsHistos','eventSelectionBscMinBiasOR')

if config.varyAttributes:
    makeAnalysis(process,'minimumBiasTTreeAnalysis','eventSelectionBscMinBiasOR',attributes)

if config.runPFlowThresholdAnalysis:
    process.pfCandidateCollNew = cms.Sequence(process.pfCandidateHFEdges+process.pfCandidateNoiseThresholds)
    process.recoSequence.replace(process.pfCandidateNoiseThresholds,process.pfCandidateCollNew)
    makeAnalysis(process,'pFlowNoiseAnalyzer','eventSelectionBscMinBiasOR')

    process.pFlowNoiseAnalyzerHFEdges = process.pFlowNoiseAnalyzer.clone( particleFlowTag = 'pfCandidateHFEdges' )
    makeAnalysis(process,'pFlowNoiseAnalyzerHFEdges','eventSelectionBscMinBiasOR')


if config.runOfflineOnly:
    makeAnalysis(process,'minimumBiasTTreeAnalysis','eventSelection')
    makeAnalysis(process,'minimumBiasTTreeAnalysis','eventSelectionBeamHaloVeto')

if config.runBPTX:
    makeAnalysis(process,'minimumBiasTTreeAnalysis','eventSelectionBPTX')
    makeAnalysis(process,'minimumBiasTTreeAnalysis','eventSelectionL1Tech4')
    makeAnalysis(process,'minimumBiasTTreeAnalysis','eventSelectionL1Tech4BeamHaloVeto') 
    makeAnalysis(process,'minimumBiasTTreeAnalysis','eventSelectionL1Tech4BscOr')
    makeAnalysis(process,'minimumBiasTTreeAnalysis','eventSelectionL1Tech4HLTBscMinBiasOR')

if config.runNoColl:
    makeAnalysis(process,'minimumBiasTTreeAnalysis','eventSelectionBscMinBiasORNoColl')
    makeAnalysis(process,'minimumBiasTTreeAnalysis','eventSelectionBscMinBiasORBPTXOR')
    #makeAnalysis(process,'pFlowNoiseAnalyzer','eventSelectionBscMinBiasORNoColl')
    #makeAnalysis(process,'pFlowNoiseAnalyzer','eventSelectionBscMinBiasORBPTXOR')

if config.runEtaMaxFilter:
    makeAnalysis(process,'minimumBiasTTreeAnalysis','eventSelectionBscMinBiasOREtaMaxFilter')
    makeAnalysis(process,'minimumBiasTTreeAnalysis','eventSelectionBscMinBiasOREtaMinFilter')

if config.runCastorFilter:
    makeAnalysis(process,'minimumBiasTTreeAnalysis','eventSelectionBscMinBiasOREtaMinFilterCastorVeto')
    makeAnalysis(process,'minimumBiasTTreeAnalysis','eventSelectionBscMinBiasOREtaMinFilterCastorTag')

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
if config.runOnMC:
    #makeAnalysis(process,'minimumBiasTTreeAnalysis','eventSelectionBscMinBiasOR',filters=processIdFilters)
    #makeAnalysis(process,'minimumBiasTTreeAnalysis','eventSelectionBscMinBiasOREtaMaxFilter',filters=processIdFilters)
    #makeAnalysis(process,'minimumBiasTTreeAnalysis','eventSelectionBscMinBiasOREtaMinFilter',filters=processIdFilters)
    #makeAnalysis(process,'minimumBiasTTreeAnalysis','eventSelectionBscMinBiasOREtaMinFilterCastorVeto',filters=processIdFilters)
    #makeAnalysis(process,'minimumBiasTTreeAnalysis','eventSelectionBscMinBiasOREtaMinFilterCastorTag',filters=processIdFilters)
    makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasOR',filters=processIdFilters)
    makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasOREtaMaxFilter',filters=processIdFilters)
    makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasOREtaMinFilter',filters=processIdFilters)
    makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasOREtaMinFilterCastorVeto',filters=processIdFilters)
    makeAnalysis(process,config.trackAnalyzerName,'eventSelectionBscMinBiasOREtaMinFilterCastorTag',filters=processIdFilters)

if config.switchPVFilter:
    from Utilities.PyConfigTools.replaceInPaths import replaceInPaths
    replaceInPaths(process,'primaryVertexFilter','primaryVertexFilterLooseNDOF0')

if config.runOnMC:
    from Utilities.PyConfigTools.removeFromPaths import removeFromPaths
    removeFromPaths(process,'bptx')
    removeFromPaths(process,'hltBscMinBiasORBptxPlusORMinusFilter')
    removeFromPaths(process,'castorInvalidDataFilter')

    from Utilities.PyConfigTools.setAnalyzerAttributes import setAnalyzerAttributes
    setAnalyzerAttributes(process,'minimumBiasTTreeAnalysis',
                                  AccessMCInfo = True,
                                  HLTPath = 'HLT_Physics_v*',
                                  CastorRecHitTag = 'castorreco') 
    #TriggerResultsTag = cms.InputTag("TriggerResults::HLT") 
