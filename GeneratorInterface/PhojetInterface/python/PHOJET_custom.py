import FWCore.ParameterSet.Config as cms

def customiseCASTOR(process):
    process.g4SimHits.Generator.MinEtaCut =-7.0
    process.g4SimHits.Generator.MaxEtaCut = 5.5

    return(process)

def customiseEventContent(process):

        AddToEventContent = cms.PSet(
            outputCommands = cms.untracked.vstring(
                'keep GenRunInfoProduct_source_*_*',
                'keep GenEventInfoProduct_source_*_*',
                'keep edmHepMCProduct_source_*_*'
            )
        )

        for moduleName in process.outputModules:
            process.outputModules[moduleName].outputCommands.extend(AddToEventContent.outputCommands)

        return(process)

def customise(process):

        process.VtxSmeared.src = "source"
        process.genParticles.src = "source"
        process.RandomNumberGeneratorService.theSource = cms.PSet(
            initialSeed = cms.untracked.uint32(123456789),
            engineName = cms.untracked.string('HepJamesRandom')
        )
        if process.g4SimHits.Generator.HepMCProductLabel == "LHCTransport":
            process.LHCTransport.HepMCProductLabel = "source"
        else:
            process.g4SimHits.Generator.HepMCProductLabel = "source"

        process.mix.mixObjects.mixHepMC.input = cms.VInputTag(cms.InputTag("source"))

        process = customiseEventContent(process)
        process = customiseCASTOR(process) 

	return(process)
