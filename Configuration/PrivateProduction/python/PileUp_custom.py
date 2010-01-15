import FWCore.ParameterSet.Config as cms

def customise(process):

        process.mix.input.nbPileupEvents.sigmaInel = 75.28
        process.mix.input.fileNames = cms.untracked.vstring(
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/02AC300B-1284-DD11-AA3D-003048344C22.root',
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/086177A2-3284-DD11-90E5-003048341AFA.root',
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/0A068579-1084-DD11-BA90-003048344B08.root',
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/0ADF2220-F383-DD11-BE35-003048344A88.root',
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/0C1C2549-1084-DD11-91CE-00304865C4A2.root',
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/0CCD3629-2C84-DD11-9E57-003048770D68.root',
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/101D7634-1084-DD11-9E92-003048770CD8.root',
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/10849E29-5484-DD11-92BE-00304865C4A4.root',
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/121047AD-0D84-DD11-9D21-003048344B08.root',
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/14AA06AF-0A84-DD11-B32C-003048770348.root',
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/16541535-2E84-DD11-922C-00304865C2A8.root',
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/1A7081A4-4684-DD11-989D-000E0C4D3CF6.root',
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/204367D0-2E84-DD11-A024-00304865C4A4.root',
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/20EA4143-1284-DD11-99D5-003048341B00.root',
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/24614ABD-0F84-DD11-BC7D-00304865C45E.root',
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/28892DC9-3084-DD11-8F38-0030487D5DAC.root',
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/2A16F348-5784-DD11-8EEA-00304865C31C.root',
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/2AFCC5BE-0A84-DD11-8408-00304865C360.root',
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/2E797386-0084-DD11-AA04-001E682F1F9A.root',
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/2EE6A6F7-1184-DD11-8D9D-000E0C4D3AAC.root',
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/30620B4C-1284-DD11-A30A-00304865C330.root',
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/3232C8D4-3184-DD11-8025-00304865C2E8.root',
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/32A115FC-1184-DD11-BA6F-00304865C2CA.root',
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/32F2DAB5-0A84-DD11-8AF9-00304865C254.root',
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/34591F9B-FF83-DD11-8632-0030487722C7.root',
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/3E68DBBD-1184-DD11-ADC5-00304865C248.root',
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/3EDFA1B5-3184-DD11-92C4-003048341A8C.root',
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/4255090E-5784-DD11-AB33-0030487B1120.root',
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/4A8C78E7-2E84-DD11-9D11-003048341A8C.root',
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/4E99BAE2-5384-DD11-8926-00304865C3D8.root',
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/4EB102CB-3384-DD11-B8EB-003048770BA8.root',
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/4ECA521B-F383-DD11-8FF5-0030487D5DAC.root',
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/4EEB5E78-3184-DD11-B394-0002B3D90783.root',
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/50BE9ABA-0B84-DD11-B22C-003048344A9A.root',
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/54B92289-0B84-DD11-8EF4-00304865C49C.root',
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/58C994BE-FF83-DD11-B426-0030482CDA64.root',
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/5C8C586E-3384-DD11-996F-003048770BAA.root',
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/5C9448A8-2E84-DD11-997A-0002B3D90783.root',
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/62A46AF1-0C84-DD11-B6AA-00304865C45E.root',
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/62F6F891-FF83-DD11-BE0C-003048344A96.root',
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/62FD095F-3384-DD11-9667-0030487D5DAC.root',
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/64EE44DB-F283-DD11-9291-003048344BA6.root',
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/6E04A6BF-4684-DD11-9B04-00304865C254.root',
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/700FEE74-0F84-DD11-9E66-003048341B00.root',
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/74394A48-0C84-DD11-8BE7-003048344C22.root',
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/74551C0C-5A84-DD11-AC92-00304865C4A4.root',
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/74A4790C-5884-DD11-BBC5-003048344C16.root',
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/769066DF-3184-DD11-B2EE-0002B3D909D5.root',
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/784A7673-5484-DD11-AE1B-00304865C4A4.root',
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/7AF0D01B-3184-DD11-A8AB-003048344C22.root',
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/809AB89C-3184-DD11-8180-00304865C4A4.root',
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/825FFEB9-5784-DD11-8458-003048341AFA.root',
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/82830FEC-2C84-DD11-8759-00304865C336.root',
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/82AD3D79-F283-DD11-A9ED-001E681E100E.root',
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/8494627F-2484-DD11-A194-0030487B1120.root',
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/86A00F25-2D84-DD11-A432-0030487B1120.root',
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/86C1DF4A-F383-DD11-9050-003048770CE0.root',
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/88CEBBDA-5984-DD11-976E-0030487B1120.root',
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/8C4E0FD9-1084-DD11-90F6-003048344A8C.root',
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/9A8EC0C1-4684-DD11-B1CA-00304865C464.root',
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/A6882BF1-2D84-DD11-B8C5-003048770BA8.root',
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/A87ADC96-3384-DD11-9B5B-0030487D5DAC.root',
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/A8C261C5-2D84-DD11-AF09-0002B3D90D81.root',
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/AA1D377E-3384-DD11-8559-00304865C456.root',
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/AC301AFF-0F84-DD11-A766-003048770BA8.root',
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/ACC1C41A-2C84-DD11-A6BC-003048341A8C.root',
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/B2FE6E36-0F84-DD11-A36D-00304865C2CA.root',
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/B41ECA3E-5784-DD11-8326-00304865C4A4.root',
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/BC7B985D-0584-DD11-B745-001E680F6620.root',
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/BEA86CA7-F983-DD11-B01E-000E0C4D3F34.root',
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/C096B27E-0F84-DD11-8275-00304865C330.root',
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/C0B127C7-3184-DD11-B9C8-003048344B08.root',
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/C2861A50-5784-DD11-AD62-003048772612.root',
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/C299148B-4684-DD11-87CA-003048344C14.root',
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/C412FCCF-5384-DD11-802F-00304865C2A8.root',
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/C62D26B6-3084-DD11-8357-003048770BA8.root',
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/C8EB4B13-0C84-DD11-8AE3-003048344A96.root',
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/CA7DC721-0C84-DD11-A866-003048344C14.root',
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/CAFD76F0-5384-DD11-9485-0030487722C7.root',
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/CC38E8C2-2D84-DD11-9ECF-0030487D5DAC.root',
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/CE7A9C3B-5384-DD11-A414-00304865C49C.root',
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/D00A3806-4784-DD11-BA3A-00304877261C.root',
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/D00B3B05-F483-DD11-BFA2-003048770BA8.root',
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/D0490F3B-3384-DD11-B49E-003048341B26.root',
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/D0B9AC6E-1084-DD11-A416-00304877A30E.root',
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/D242F390-3084-DD11-BACF-0030487D5DAC.root',
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/D4997118-3184-DD11-A09D-00304865C45E.root',
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/D4BB40E9-0F84-DD11-B5DF-003048770B8C.root',
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/D88DD4D9-F683-DD11-8238-000E0C4D3F34.root',
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/D8ADF552-0F84-DD11-A474-003048344C22.root',
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/D8CFFAB5-5784-DD11-BEA5-00304861D446.root',
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/DCB612A6-2D84-DD11-AAFF-003048341B26.root',
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/E0723EAF-3384-DD11-A6A6-00304865C336.root',
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/E2136B1B-0C84-DD11-90F9-003048770DC0.root',
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/E489AB70-3084-DD11-89DB-003048341B26.root',
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/E8227E0D-1084-DD11-B09F-003048341C62.root',
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/EEC7CE08-5A84-DD11-B2B7-003048772648.root',
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/F2669E6F-5384-DD11-B549-003048344C16.root',
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/FC5FD123-0A84-DD11-8375-00304865C45E.root',
               '/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0031/FCB17EF8-5384-DD11-AC1A-003048344BA6.root'
        ) 

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