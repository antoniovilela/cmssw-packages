import FWCore.ParameterSet.Config as cms

def customise(process):

        process.mix.bunchspace = 50
        process.mix.input.nbPileupEvents.sigmaInel = 75.28
        process.mix.input.nbPileupEvents.Lumi = 0.11
        from Configuration.GenProduction.fileNames_MinBias_IDEAL_V11_redigi_v1_RAW_750002d8_0f67_402f_84c5_039079c48f87_cfi import filesPSet
        process.mix.input.fileNames = filesPSet.fileNames

        process.MyEventContent = cms.PSet(
               outputCommands = cms.untracked.vstring('drop *')
        )
        process.MyEventContent.outputCommands.extend(process.RAWSIMEventContent.outputCommands)
        process.MyEventContent.outputCommands.extend(process.SimGeneralFEVTDEBUG.outputCommands)
        process.MyEventContent.outputCommands.extend(process.SimTrackerFEVTDEBUG.outputCommands)
        process.MyEventContent.outputCommands.append("keep CrossingFramePlaybackInfo_mix_*_*")
        process.MyEventContent.outputCommands.append("keep edmHepMCProductCrossingFrame_mix_*_*")

        process.output.outputCommands = process.MyEventContent.outputCommands

        process.genParticles.abortOnUnknownPDGCode = False

	return(process)
