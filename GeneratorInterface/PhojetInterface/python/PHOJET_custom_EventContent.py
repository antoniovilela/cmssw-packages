import FWCore.ParameterSet.Config as cms

def customise(process):

        AddToEventContent = cms.PSet(
            outputCommands = cms.untracked.vstring(
                'keep GenRunInfoProduct_source_*_*',
                'keep GenEventInfoProduct_source_*_*',
                'keep edmHepMCProduct_source_*_*'
            )
        )   

        process.output.outputCommands.extend(AddToEventContent.outputCommands)

	return(process)
