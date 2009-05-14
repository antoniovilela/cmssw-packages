import FWCore.ParameterSet.Config as cms

def customise(process):

        process.mix.bunchspace = 450
        process.mix.input.nbPileupEvents.sigmaInel = 75.28
        process.mix.input.nbPileupEvents.Lumi = 0.049
        from Configuration.GenProduction.fileNames_MinBias_IDEAL_V11_redigi_v1_RAW_750002d8_0f67_402f_84c5_039079c48f87_cfi import filesPSet
        process.mix.input.fileNames = filesPSet.fileNames

        # SimTransport
        process.load("SimTransport.HectorProducer.HectorTransport_cfi")
        process.LHCTransport.ZDCTransport = False
        process.g4SimHits.Generator.HepMCProductLabel = 'LHCTransport'

        # Digi for FP420
        process.load("SimRomanPot.SimFP420.FP420Digi_cfi")
        process.FP420Digi.ApplyTofCut = False

        process.genParticles.abortOnUnknownPDGCode = False

        process.MyEventContent = cms.PSet(
               outputCommands = cms.untracked.vstring('drop *')
        )
        process.MyEventContent.outputCommands.extend(process.RAWSIMEventContent.outputCommands)
        process.MyEventContent.outputCommands.extend(process.SimGeneralFEVTDEBUG.outputCommands)
        process.MyEventContent.outputCommands.extend(process.SimTrackerFEVTDEBUG.outputCommands)
        process.MyEventContent.outputCommands.append("keep CrossingFramePlaybackInfo_mix_*_*")
        process.MyEventContent.outputCommands.append("keep edmHepMCProductCrossingFrame_mix_*_*")

        process.MyEventContent.outputCommands.append('keep *_FP420Digi_*_*')
        process.MyEventContent.outputCommands.append('keep *_LHCTransport_*_*')

        process.output.outputCommands = process.MyEventContent.outputCommands

        process.psim_new = cms.Sequence(process.LHCTransport*process.psim)
        process.pdigi_new = cms.Sequence(process.pdigi*process.FP420Digi)

        process.simulation_step.replace(process.psim,process.psim_new)
        process.digitisation_step.replace(process.pdigi,process.pdigi_new)

	return(process)
