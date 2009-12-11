import FWCore.ParameterSet.Config as cms

def customise(process):

        process.VtxSmeared.src = "source"
        process.genParticles.src = "source"
        process.RandomNumberGeneratorService.theSource = cms.PSet(
            initialSeed = cms.untracked.uint32(123456789),
            engineName = cms.untracked.string('HepJamesRandom')
        )
        process.LHCTransport.HepMCProductLabel = "source"
        process.mix.mixObjects.mixHepMC.input = cms.VInputTag(cms.InputTag("source"))

	return(process)
