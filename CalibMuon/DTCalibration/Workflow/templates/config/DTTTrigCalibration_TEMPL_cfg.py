import FWCore.ParameterSet.Config as cms

process = cms.Process("TTRIGCALIBPROC")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = 'WARNING'
#process.MessageLogger.cerr.FwkReport.reportEvery = 1000

process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = "@@GLOBALTAG@@"

process.load("CondCore.DBCommon.CondDBSetup_cfi")

process.source = cms.Source("PoolSource",
    debugFlag = cms.untracked.bool(True),
    debugVebosity = cms.untracked.uint32(10),
    fileNames = cms.untracked.vstring()
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(10000)
)

process.load("CalibMuon.DTCalibration.DTTTrigCalibration_cfi")
process.ttrigcalib.rootFileName = 'DTTimeBoxes.root'
process.ttrigcalib.kFactor = -0.7
process.ttrigcalib.digiLabel = '@@MUDIGILABEL@@'

# if read from RAW
#process.load("EventFilter.DTRawToDigi.dtunpacker_cfi")

process.PoolDBOutputService = cms.Service("PoolDBOutputService",
    DBParameters = cms.PSet(
        authenticationPath = cms.untracked.string('/afs/cern.ch/cms/DB/conddb')
    ),
    authenticationMethod = cms.untracked.uint32(0),
    connect = cms.string('sqlite_file:ttrig.db'),
    toPut = cms.VPSet(cms.PSet(
        record = cms.string('DTTtrigRcd'),
        tag = cms.string('ttrig')
    ))
)

process.p = cms.Path(process.ttrigcalib)

# if read from RAW
#process.p = cms.Path(process.muonDTDigis*process.ttrigcalib)
