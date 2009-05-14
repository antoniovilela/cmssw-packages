import FWCore.ParameterSet.Config as cms

def customise(process):

       # FP420
       process.load("RecoRomanPot.RecoFP420.FP420Cluster_cfi")
       process.load("RecoRomanPot.RecoFP420.FP420Track_cfi")
       process.load("RecoRomanPot.RecoFP420.FP420Reco_cfi") 

       process.MyEventContent = cms.PSet(
           process.RECOSIMEventContent
       ) 
       process.MyEventContent.outputCommands.append('keep DigiCollectionFP420_*_*_*')
       process.MyEventContent.outputCommands.append('keep ClusterCollectionFP420_*_*_*')
       process.MyEventContent.outputCommands.append('keep TrackCollectionFP420_*_*_*')
       process.MyEventContent.outputCommands.append('keep RecoCollectionFP420_*_*_*')
 
       process.output.outputCommands = process.MyEventContent.outputCommands

       process.reconstruction_new = cms.Sequence(process.reconstruction*
                                                 process.FP420Cluster*
                                                 process.FP420Track*
                                                 process.FP420Reco)
   
       process.reconstruction_step.replace(process.reconstruction,process.reconstruction_new)

       return(process)
