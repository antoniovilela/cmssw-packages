import FWCore.ParameterSet.Config as cms

def customise(process):

        process.VtxSmeared.src = "source"
        process.genParticles.src = "source"
        process.RandomNumberGeneratorService.theSource = cms.PSet(
            initialSeed = cms.untracked.uint32(123456789),
            engineName = cms.untracked.string('HepJamesRandom')
        )

	return(process)
