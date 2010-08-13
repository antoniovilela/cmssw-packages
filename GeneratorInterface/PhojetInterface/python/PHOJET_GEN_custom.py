import FWCore.ParameterSet.Config as cms

def customiseEventContent(process):

        AddToEventContent = cms.PSet(
            outputCommands = cms.untracked.vstring(
                'keep GenRunInfoProduct_source_*_*',
                'keep GenEventInfoProduct_source_*_*',
                'keep edmHepMCProduct_source_*_*'
            )
        )

        process.output.outputCommands.extend(AddToEventContent.outputCommands)

        return(process)

def customise(process):

        process.VtxSmeared.src = "source"
        process.genParticles.src = "source"
        process.RandomNumberGeneratorService.theSource = cms.PSet(
            initialSeed = cms.untracked.uint32(123456789),
            engineName = cms.untracked.string('HepJamesRandom')
        )

        process = customiseEventContent(process)
 
	return(process)
