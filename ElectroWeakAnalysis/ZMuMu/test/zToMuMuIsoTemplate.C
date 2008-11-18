#include "TH1F.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TProfile.h"
#include "TString.h"
#include "TFile.h"
#include "TStyle.h"
#include "TChain.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TLegend.h"
#include "TRandom.h"
#include "TTree.h"
#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>
// Trick to make CINT happy
//#include "DataFormats/FWLite/interface/Handle.h"
//#include "DataFormats/FWLite/interface/Event.h"
#if !defined(__CINT__) && !defined(__MAKECINT__)
//Headers for the data items
#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/FWLite/interface/Event.h"
#include "DataFormats/Common/interface/ValueMap.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Candidate/interface/CandidateFwd.h"
//#include "DataFormats/RecoCandidate/interface/RecoCandidate.h"
#include "DataFormats/Candidate/interface/CompositeCandidate.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/MuonReco/interface/MuIsoDeposit.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
//#include "DataFormats/METReco/interface/CaloMET.h"
#endif

//main
static const double wMass = 80.403; // PDG W mass
static const double zMass = 91.1876; // PDG Z mass
void zToMuMuIsoTemplate(int maxEvents = -1, bool verbose = false) {
   TH1::SetDefaultSumw2(true);
   gRandom->SetSeed(0);
   // Create a vector of input files
   vector<string> fileNames;
   //fileNames.push_back("rfio:/castor/cern.ch/user/a/antoniov/Analysis/zToMuMu/zToMuMuNew_ReReco10pb.root");
   //fileNames.push_back("/tmp/antoniov/zToMuMu_1k.root");
   fileNames.push_back("/tmp/antoniov/zToMuMuSkim_ReReco10pb.root");
   
   // Chain the input files
   fwlite::ChainEvent ev(fileNames);
   
   // Create output file
   TFile* hfile = new TFile("ZmumuIsoTemplate.root","recreate","data histograms");

   // Book Histograms
   TH1F* hMuIsofromZmumu = new TH1F("hMuIsofromZmumu","hMuIsofromZmumu",50, 0., 10.);
   TH1F* hMuNormIsofromZmumu = new TH1F("hMuNormIsofromZmumu","hMuNormIsofromZmumu",50, 0., 0.3);
   TH2F* hMuIsovsVBPt = new TH2F("hMuIsovsVBPt","hMuIsovsVBPt",60,0.,80.,50,0.,10.);
   TH2F* hMuIsovsMuPt = new TH2F("hMuIsovsMuPt","hMuIsovsMuPt",50,0.,100.,50,0.,10.);
   TH2F* hMuNormIsovsVBPt = new TH2F("hMuNormIsovsVBPt","hMuNormIsovsVBPt",60,0.,80.,50,0.,0.3);
   TH2F* hMuNormIsovsMuPt = new TH2F("hMuNormIsovsMuPt","hMuNormIsovsMuPt",50,0.,100.,50,0.,0.3);	
   
   TH2F* hMuIsovsMuPtEta = new TH2F("hMuIsovsMuPtEta","hMuIsovsMuPtEta",50,0.,100.,50,0.,2.);
   TH2F* hMuNormIsovsMuPtEta = new TH2F("hMuNormIsovsMuPtEta","hMuNormIsovsMuPtEta",50,0.,100.,50,0.,2.);	
   TH3F* hMuIsovsMuPtvsMuEta = new TH3F("hMuIsovsMuPtvsMuEta","hMuIsovsMuPtvsMuEta",10,0.,100.,5,0.,2.0,50,0.,10.);
   TH3F* hMuNormIsovsMuPtvsMuEta = new TH3F("hMuNormIsovsMuPtvsMuEta","hMuNormIsovsMuPtvsMuEta",10,0.,100.,5,0.,2.0,50,0.,0.3);
 
   //double tree_PtNu,tree_EtaNu,tree_VBPt,tree_VBEta,tree_VBPhi,tree_METfromZmumu,tree_MTfromZmumu;
   double tree_muonPt,tree_muonEta,tree_muonPhi;	
   double tree_VBPt,tree_VBEta,tree_VBPhi;
   double tree_PtSum,tree_NormIso;

   TTree* data = new TTree("data","data");
   /*data->Branch("PtNu",&tree_PtNu,"PtNu/D");
   data->Branch("EtaNu",&tree_EtaNu,"EtaNu/D");
   data->Branch("VBPt",&tree_VBPt,"VBPt/D");
   data->Branch("VBEta",&tree_VBEta,"VBEta/D");
   data->Branch("VBPhi",&tree_VBPhi,"VBPhi/D");
   data->Branch("METfromZmumu",&tree_METfromZmumu,"METfromZmumu/D");
   data->Branch("MTfromZmumu",&tree_MTfromZmumu,"MTfromZmumu/D"); */
   data->Branch("muonPt",&tree_muonPt,"muonPt/D");
   data->Branch("muonEta",&tree_muonEta,"muonEta/D");
   data->Branch("muonPhi",&tree_muonPhi,"muonPhi/D");
   data->Branch("VBPt",&tree_VBPt,"VBPt/D");
   data->Branch("VBEta",&tree_VBEta,"VBEta/D");
   data->Branch("VBPhi",&tree_VBPhi,"VBPhi/D");
   data->Branch("PtSum",&tree_PtSum,"PtSum/D");
   data->Branch("NormIso",&tree_NormIso,"NormIso/D");

   // Z Selection cuts
   double ptCutTight_ = 20.0;
   double ptCutLoose_ = 20.0;
   double etaCut_ = 2.0;
   float isoCut_ = 0.09;
   float isoCone_ = 0.3;	
   double minZmass = 70.0;
   double maxZmass = 110.0; 			
   // Loop over the events
   int nEvts = 0;
   for( ev.toBegin(); ! ev.atEnd(); ++ev) {

     if(nEvts == maxEvents) break;	
     nEvts++;
     // Get the collections
     if(verbose) std::cout << ">>> Event number: " << nEvts << endl;	
     // Z candidates
     fwlite::Handle<reco::CompositeCandidateCollection> zMuMu; 
     zMuMu.getByLabel(ev,"goodZToMuMu");

     // Corrected MET	
     //fwlite::Handle<reco::CaloMETCollection> metColl;
     //metColl.getByLabel(ev,"corMetType1Icone5");
     //metColl.getByLabel(ev,"corMetMuonCorrJetCorr");	
     //metColl.getByLabel(ev,"corMetGlobalMuons");
     //metColl.getByLabel(ev,"met");		
	
     /*const reco::CaloMET& met = metColl->front();

     double met_px = met.px();
     double met_py = met.py();

     // Do muon subtraction
     fwlite::Handle<reco::MuonCollection> muonColl;
     muonColl.getByLabel(ev,"muons");
     for(size_t imu = 0; imu < muonColl->size(); ++imu){
	const reco::Muon& mu = (*muonColl)[imu];
	met_px -= mu.px();
	met_py -= mu.py();
     }*/		
			 
     // Isolations
     //fwlite::Handle<edm::ValueMap<float> > muonIso;
     //muonIso.getByLabel(ev,"muonIsolations");
     fwlite::Handle<reco::MuIsoDepositAssociationMap> isodepMap;
     isodepMap.getByLabel(ev,"muGlobalIsoDepositCtfTk");	
	
     //const edm::ValueMap<float>& muIsolations = *muonIso;

     // Do W selection for Z events (only one muon isolated with pt > 25.0)
     bool eventSelected = false;

     //std::cout << "Number of Z candidates: " << zMuMu->size() << endl;	
     if(zMuMu->size() < 1) continue;	

     if(verbose) std::cout << "beginning z loop" << std::endl;
     int iselZ = -1;
     double aux_mass = 999999.;	
     const reco::Candidate* muonTight = 0;
     const reco::Candidate* muonLoose = 0;			      
     for(size_t iter = 0; iter < zMuMu->size(); ++iter){
	const reco::Candidate& zmmCand = (*zMuMu)[iter];
	reco::MuonRef muon1 = zmmCand.daughter(0)->masterClone().castTo<reco::MuonRef>();
	reco::MuonRef muon2 = zmmCand.daughter(1)->masterClone().castTo<reco::MuonRef>();

	/*float iso1 = muIsolations[muon1];
	iso1 = iso1/muon1->pt();
        float iso2 = muIsolations[muon2];
	iso2 = iso2/muon2->pt();*/
	reco::TrackRef muTrack1 = muon1->combinedMuon();
	const reco::MuIsoDeposit dep1 = (*isodepMap)[muTrack1];
	float iso1 = dep1.depositWithin(isoCone_);
	iso1 = iso1/muon1->pt();
	reco::TrackRef muTrack2 = muon2->combinedMuon();
        const reco::MuIsoDeposit dep2 = (*isodepMap)[muTrack2];
        float iso2 = dep2.depositWithin(isoCone_);
        iso2 = iso2/muon2->pt();
	if(verbose) std::cout << "muon 1 pt,eta,iso= " << muon1->pt() << " , " << muon1->eta() << " , " << iso1 << std::endl;
	if(verbose) std::cout << "muon 2 pt,eta,iso= " << muon2->pt() << " , " << muon2->eta() << " , " << iso2 << std::endl;

	// Require mass window
	if((zmmCand.mass() < minZmass)||(zmmCand.mass() > maxZmass)) continue;

	// Require that dR(mu1,mu2) > isoCone 
	//if(deltaR(*muon1,*muon2) < isoCone_) continue;

	/*bool mu1tight = ((muon1->pt() > ptCutTight_)&&(fabs(muon1->eta()) < etaCut_)&&(iso1 < isoCut_));
	bool mu2tight = ((muon2->pt() > ptCutTight_)&&(fabs(muon2->eta()) < etaCut_)&&(iso2 < isoCut_));

	bool mu1loose = ((muon1->pt() > ptCutLoose_)&&(fabs(muon1->eta()) < etaCut_));
	bool mu2loose = ((muon2->pt() > ptCutLoose_)&&(fabs(muon2->eta()) < etaCut_));

	if((mu1tight&&mu2tight)||(mu1tight&&mu2loose)||(mu1loose&&mu2tight)){
		eventSelected = true;
		if(fabs(zmmCand.mass() - zMass) < fabs(aux_mass - zMass)){
                        aux_mass = zmmCand.mass();
			iselZ = iter;
			if(mu1tight&&mu2tight){
				if(gRandom->Rndm() < 0.5){
					muonTight = zmmCand.daughter(0);
                                	muonLoose = zmmCand.daughter(1);
				} else{
					muonTight = zmmCand.daughter(1);
                                        muonLoose = zmmCand.daughter(0);
				}
			} else if(mu1tight&&mu2loose){
				muonTight = zmmCand.daughter(0);
				muonLoose = zmmCand.daughter(1);
			} else if(mu1loose&&mu2tight){
				muonTight = zmmCand.daughter(1);
                                muonLoose = zmmCand.daughter(0);
			} else {std::cout << ">>>> Not valid option!!" << std::endl;return;}
                }
	}*/
	bool mu1loose = ((muon1->pt() > ptCutLoose_)&&(fabs(muon1->eta()) < etaCut_));
        bool mu2loose = ((muon2->pt() > ptCutLoose_)&&(fabs(muon2->eta()) < etaCut_));

	if((!mu1loose)||(!mu2loose)) continue;

	bool mu1tight = ((muon1->pt() > ptCutTight_)&&(fabs(muon1->eta()) < etaCut_)&&(iso1 < isoCut_));
        bool mu2tight = ((muon2->pt() > ptCutTight_)&&(fabs(muon2->eta()) < etaCut_)&&(iso2 < isoCut_));

	if(mu1tight||mu2tight){
		eventSelected = true;
                if(fabs(zmmCand.mass() - zMass) < fabs(aux_mass - zMass)){
                        aux_mass = zmmCand.mass();
                        iselZ = iter;
			if(mu1tight&&mu2tight){
                                if(gRandom->Rndm() < 0.5){
                                        muonTight = zmmCand.daughter(0);
                                        muonLoose = zmmCand.daughter(1);
                                } else{
                                        muonTight = zmmCand.daughter(1);
                                        muonLoose = zmmCand.daughter(0);
                                }
                        } else if(mu1tight&&(!mu2tight)){
				muonTight = zmmCand.daughter(0);
                                muonLoose = zmmCand.daughter(1);
			} else if(mu2tight&&(!mu1tight)){
				muonTight = zmmCand.daughter(1);
                                muonLoose = zmmCand.daughter(0);
			} else {std::cout << ">>>> Not valid option!!" << std::endl;return;}
		}	
	}
     }
     if(verbose) std::cout << "after z loop" << std::endl;
     if(!eventSelected) continue;
     assert(iselZ >= 0);	
	
     /*const reco::Candidate& myZ = (*zMuMu)[iselZ];
     assert(myZ.numberOfDaughters() == 2);
     const reco::Candidate* mu1 = myZ.daughter(0);
     const reco::Candidate* mu2 = myZ.daughter(1);
     if((mu1 == 0)||(mu2 == 0)) continue;*/

     const reco::Candidate& myZ = (*zMuMu)[iselZ];
     assert(myZ.numberOfDaughters() == 2);
	
     if((muonTight == 0)||(muonLoose == 0)) continue;	
     // Calculate MET from Z
     // First sum back the "neutrino"
     //double metFromZ_px = met.px() + neutrino->px();
     //double metFromZ_py = met.py() + neutrino->py();			
     /*double metFromZ_px = met_px + neutrino->px();
     double metFromZ_py = met_py + neutrino->py();		
     double metFromZ_pt = sqrt(metFromZ_px*metFromZ_px + metFromZ_py*metFromZ_py);
     math::XYZTLorentzVector metFromZ(metFromZ_px,metFromZ_py,0.,metFromZ_pt);*/

     // Calculate "W" MT
     /*double w_et = muon->pt() + metFromZ.pt();
     double w_px = muon->px() + metFromZ.px();
     double w_py = muon->py() + metFromZ.py();
     double massT = w_et*w_et - w_px*w_px - w_py*w_py;
     massT = (massT>0) ? sqrt(massT) : 0;
 		
     hMETfromZmumu->Fill(metFromZ.pt()*(wMass/zMass));
     hMTfromZmumu->Fill(massT*(wMass/zMass));*/

     reco::MuonRef muloose = (muonLoose->masterClone()).castTo<reco::MuonRef>();
     //float PtSumFromZmumu = muIsolations[muloose];
     reco::TrackRef mutrk = muloose->combinedMuon();
     const reco::MuIsoDeposit dep = (*isodepMap)[mutrk];
     float PtSumFromZmumu = dep.depositWithin(isoCone_);	
     float NormIsoFromZmumu = PtSumFromZmumu/muloose->pt();
	 
     hMuIsofromZmumu->Fill(PtSumFromZmumu*(wMass/zMass));
     hMuNormIsofromZmumu->Fill(NormIsoFromZmumu);
     hMuIsovsVBPt->Fill((myZ.pt()*(wMass/zMass)),PtSumFromZmumu*(wMass/zMass));	
     hMuIsovsMuPt->Fill(muloose->pt()*(wMass/zMass),PtSumFromZmumu*(wMass/zMass));
     hMuNormIsovsVBPt->Fill((myZ.pt()*(wMass/zMass)),NormIsoFromZmumu);
     hMuNormIsovsMuPt->Fill(muloose->pt()*(wMass/zMass),NormIsoFromZmumu);	
  
     hMuIsovsMuPtEta->Fill(muloose->pt()*(wMass/zMass),fabs(muloose->eta()),PtSumFromZmumu*(wMass/zMass));
     hMuNormIsovsMuPtEta->Fill(muloose->pt()*(wMass/zMass),fabs(muloose->eta()),NormIsoFromZmumu);
     hMuIsovsMuPtvsMuEta->Fill(muloose->pt()*(wMass/zMass),fabs(muloose->eta()),PtSumFromZmumu*(wMass/zMass));
     hMuNormIsovsMuPtvsMuEta->Fill(muloose->pt()*(wMass/zMass),fabs(muloose->eta()),NormIsoFromZmumu);
		
     /*tree_PtNu = neutrino->pt();
     tree_EtaNu = neutrino->eta();
     tree_VBPt = myZ.pt()*(wMass/zMass);
     tree_VBEta = myZ.eta();
     tree_VBPhi = myZ.phi();
     tree_METfromZmumu = metFromZ.pt()*(wMass/zMass);
     tree_MTfromZmumu = massT*(wMass/zMass);*/
     tree_muonPt = muloose->pt();
     tree_muonEta = muloose->eta();	
     tree_muonPhi = muloose->phi();
     tree_VBPt = myZ.pt()*(wMass/zMass);
     tree_VBEta = myZ.eta();
     tree_VBPhi = myZ.phi();	
     tree_PtSum = PtSumFromZmumu;
     tree_NormIso = NormIsoFromZmumu;		 

     data->Fill();
   }  // End loop over events
  
  // Write output file
  hfile->cd();	
  hfile->Write();
  hfile->Close();

}

