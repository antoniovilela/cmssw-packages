import FWCore.ParameterSet.Config as cms
from ExclusiveDijetsAnalysis.ExclusiveDijetsAnalysis.analysisTools import parseInput
 
inputFields = ('fileIn','fileAnalysisOut','accessPileUpInfo','useHFTowerWeighted')
requiredFields = ()

input = parseInput(inputFields,requiredFields)

# Treat default case
if not hasattr(input,'fileIn'):
    input.fileIn = 'file:pool.root'
else:
    if input.fileIn.find(':') == -1: input.fileIn = 'file:' + input.fileIn

if not hasattr(input,'fileAnalysisOut'): input.fileAnalysisOut = 'analysisDijets_TTree.root'
if not hasattr(input,'accessPileUpInfo'): input.accessPileUpInfo = False
if not hasattr(input,'useHFTowerWeighted'): input.useHFTowerWeighted = True

# Print parameters
for item in inputFields:
    print "Using %s = %s" % (item,getattr(input,item))

# Build cms.Process
process = cms.Process("ExclusiveDijetsAnalysis")

process.load('ExclusiveDijetsAnalysis.ExclusiveDijetsAnalysis.messageLogger_cfi')
process.MessageLogger.cerr.threshold = 'INFO'

process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(input.fileIn)
)

#process.load("JetMETCorrections.Configuration.L2L3Corrections_Summer08Redigi_cff")

from ExclusiveDijetsAnalysis.ExclusiveDijetsAnalysis.scaledPATJets_cfi import scaledPATJets 
scaledPATJets.src = "selectedLayer1JetsSC7PF"
process.scaledPATJets1pt1 = scaledPATJets.clone(scale = 1.10)
process.scaledPATJets0pt9 = scaledPATJets.clone(scale = 0.90)
process.scaledJets_step = cms.Path(process.scaledPATJets1pt1+process.scaledPATJets0pt9)

process.load("ExclusiveDijetsAnalysis.ExclusiveDijetsAnalysis.exclusiveDijetsFilter_cfi")
process.load("ExclusiveDijetsAnalysis.ExclusiveDijetsAnalysis.exclusiveDijetsTTreeAnalysis_cfi")
process.exclusiveDijetsTTreeAnalysis.AccessPileUpInfo = input.accessPileUpInfo
process.exclusiveDijetsTTreeAnalysis.UseHFTowerWeighted = input.useHFTowerWeighted

process.load("DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.singleVertexFilter_cfi")
process.exclusiveDijetsSelection = cms.Sequence(process.singleVertexFilter+process.exclusiveDijetsFilter)

filters = []
attributes = [{'JetTag':'scaledPATJets1pt1'},
              {'JetTag':'scaledPATJets0pt9'}]

from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.analysisTools import *
makeAnalysis(process,'exclusiveDijetsTTreeAnalysis','exclusiveDijetsSelection',attributes,filters)

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string(input.fileAnalysisOut)
)
