import FWCore.ParameterSet.Config as cms

def customise(process):

        # SimTransport
        process.load("SimTransport.HectorProducer.HectorTransport_cfi")
        process.LHCTransport.ZDCTransport = False
        process.g4SimHits.Generator.HepMCProductLabel = 'LHCTransport'

        # Digi for FP420
        process.load("SimRomanPot.SimFP420.FP420Digi_cfi")
        process.FP420Digi.ApplyTofCut = False

        process.genParticles.abortOnUnknownPDGCode = False

        process.MyEventContent = cms.PSet(
            process.RAWSIMEventContent
        )
        process.MyEventContent.outputCommands.append('keep *_FP420Digi_*_*')
        process.MyEventContent.outputCommands.append('keep *_LHCTransport_*_*')

        process.output.outputCommands = process.MyEventContent.outputCommands

        process.psim_new = cms.Sequence(process.LHCTransport*process.psim)
        process.pdigi_new = cms.Sequence(process.pdigi*process.FP420Digi)

        process.simulation_step.replace(process.psim,process.psim_new)
        process.digitisation_step.replace(process.pdigi,process.pdigi_new)

	return(process)
