import FWCore.ParameterSet.Config as cms

def customise(process):

        process.MyEventContent = cms.PSet(
               outputCommands = cms.untracked.vstring('drop *')
        )
        process.MyEventContent.outputCommands.extend(process.RECOSIMEventContent.outputCommands)
        process.MyEventContent.outputCommands.extend(process.SimGeneralFEVTDEBUG.outputCommands)
        process.MyEventContent.outputCommands.extend(process.SimTrackerFEVTDEBUG.outputCommands)
        process.MyEventContent.outputCommands.append("keep CrossingFramePlaybackInfo_mix_*_*")
        process.MyEventContent.outputCommands.append("keep edmHepMCProductCrossingFrame_mix_*_*")

        process.output.outputCommands = process.MyEventContent.outputCommands

	return(process)
