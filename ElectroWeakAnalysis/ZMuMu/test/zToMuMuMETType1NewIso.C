#include "TH1F.h"
#include "TH2F.h"
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
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Candidate/interface/CompositeCandidate.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/METReco/interface/CaloMET.h"
#endif

//main
static const double wMass = 80.403; // PDG W mass
static const double zMass = 91.1876; // PDG Z mass
void zToMuMuMETType1NewIso(int maxEvents = -1, bool verbose = false) {
   TH1::SetDefaultSumw2(true);
   gRandom->SetSeed(0);
   // Create a vector of input files
   vector<string> fileNames;
   //fileNames.push_back("rfio:/castor/cern.ch/user/a/antoniov/Analysis/zToMuMu/zToMuMuNew_ReReco10pb.root");
   //fileNames.push_back("/tmp/antoniov/zToMuMu_1k.root");
   fileNames.push_back("/tmp/antoniov/zToMuMuSkim.root");
   
   // Chain the input files
   fwlite::ChainEvent ev(fileNames);
   
   // Create output file
   TFile* hfile = new TFile("ZMETType1NewIso_noMuonCorr_noJetCorr.root","recreate","data histograms");

   // Book Histograms
   TH1F* hMETfromZmumu = new TH1F("hMETfromZmumu","hMETfromZmumu",100, 0., 100.);
   TH1F* hMTfromZmumu = new TH1F("hMTfromZmumu","hMTfromZmumu",100, 0., 200.);	

   double tree_PtNu,tree_EtaNu,tree_VBPt,tree_VBEta,tree_VBPhi,tree_METfromZmumu,tree_MTfromZmumu;

   TTree* data = new TTree("data","data");
   data->Branch("PtNu",&tree_PtNu,"PtNu/D");
   data->Branch("EtaNu",&tree_EtaNu,"EtaNu/D");
   data->Branch("VBPt",&tree_VBPt,"VBPt/D");
   data->Branch("VBEta",&tree_VBEta,"VBEta/D");
   data->Branch("VBPhi",&tree_VBPhi,"VBPhi/D");
   data->Branch("METfromZmumu",&tree_METfromZmumu,"METfromZmumu/D");
   data->Branch("MTfromZmumu",&tree_MTfromZmumu,"MTfromZmumu/D"); 

   // Z Selection cuts
   double ptCutTight_ = 25.0*(zMass/wMass);
   double ptCutLoose_ = 20.0;
   double etaCut_ = 2.0;
   float isoCut_ = 0.09;
   double minZmass = 80.0;
   double maxZmass = 100.0; 			
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
     fwlite::Handle<reco::CaloMETCollection> metColl;
     //metColl.getByLabel(ev,"corMetType1Icone5");
     //metColl.getByLabel(ev,"corMetMuonCorrJetCorr");	
     //metColl.getByLabel(ev,"corMetGlobalMuons");
     metColl.getByLabel(ev,"met");		
	
     const reco::CaloMET& met = metColl->front();

     double met_px = met.px();
     double met_py = met.py();

     // Do muon subtraction
     fwlite::Handle<reco::MuonCollection> muonColl;
     muonColl.getByLabel(ev,"muons");
     for(size_t imu = 0; imu < muonColl->size(); ++imu){
	const reco::Muon& mu = (*muonColl)[imu];
	met_px -= mu.px();
	met_py -= mu.py();
     }		
			 
     // Isolations
     fwlite::Handle<edm::ValueMap<float> > muonIso;
     muonIso.getByLabel(ev,"muonIsolations");
	
     const edm::ValueMap<float>& muIsolations = *muonIso;

     // Do W selection for Z events (only one muon isolated with pt > 25.0)
     bool eventSelected = false;

     //std::cout << "Number of Z candidates: " << zMuMu->size() << endl;	
     if(zMuMu->size() < 1) continue;	

     if(verbose) std::cout << "beginning z loop" << std::endl;
     int iselZ = -1;
     double aux_mass = 999999.;	
     const reco::Candidate* muon = 0;
     const reco::Candidate* neutrino = 0;			      
     for(size_t iter = 0; iter < zMuMu->size(); ++iter){
	const reco::Candidate& zmmCand = (*zMuMu)[iter];
	reco::MuonRef muon1 = zmmCand.daughter(0)->masterClone().castTo<reco::MuonRef>();
	reco::MuonRef muon2 = zmmCand.daughter(1)->masterClone().castTo<reco::MuonRef>();
	
	float iso1 = muIsolations[muon1];
	iso1 = iso1/muon1->pt();
        float iso2 = muIsolations[muon2];
	iso2 = iso2/muon2->pt();
	if(verbose) std::cout << "muon 1 pt,eta,iso= " << muon1->pt() << " , " << muon1->eta() << " , " << iso1 << std::endl;
	if(verbose) std::cout << "muon 2 pt,eta,iso= " << muon2->pt() << " , " << muon2->eta() << " , " << iso2 << std::endl;

	/*if(((muon1->pt() > ptCut_)&&(fabs(muon1->eta()) < etaCut_)&&(iso1 < isoCut_))&&
	   ((muon2->pt() > ptCut_)&&(fabs(muon2->eta()) < etaCut_)&&(iso2 < isoCut_))&&
	   (zmmCand.mass() > minZmass)&&(zmmCand.mass() < maxZmass)){
		eventSelected = true;
		if(fabs(zmmCand.mass() - zMass) < fabs(aux_mass - zMass)){
			aux_mass = zmmCand.mass();
			iselZ = iter;
		} 
	}*/

	if((zmmCand.mass() < minZmass)||(zmmCand.mass() > maxZmass)) continue;

	bool mu1tight = ((muon1->pt() > ptCutTight_)&&(fabs(muon1->eta()) < etaCut_)&&(iso1 < isoCut_));
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
					muon = zmmCand.daughter(0);
                                	neutrino = zmmCand.daughter(1);
				} else{
					muon = zmmCand.daughter(1);
                                        neutrino = zmmCand.daughter(0);
				}
			} else if(mu1tight&&mu2loose){
				muon = zmmCand.daughter(0);
				neutrino = zmmCand.daughter(1);
			} else if(mu1loose&&mu2tight){
				muon = zmmCand.daughter(1);
                                neutrino = zmmCand.daughter(0);
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
	
     if((muon == 0)||(neutrino == 0)) continue;	
     // Calculate MET from Z
     // First sum back the "neutrino"
     //double metFromZ_px = met.px() + neutrino->px();
     //double metFromZ_py = met.py() + neutrino->py();			
     double metFromZ_px = met_px + neutrino->px();
     double metFromZ_py = met_py + neutrino->py();		
     double metFromZ_pt = sqrt(metFromZ_px*metFromZ_px + metFromZ_py*metFromZ_py);
     math::XYZTLorentzVector metFromZ(metFromZ_px,metFromZ_py,0.,metFromZ_pt);

     // Calculate "W" MT
     double w_et = muon->pt() + metFromZ.pt();
     double w_px = muon->px() + metFromZ.px();
     double w_py = muon->py() + metFromZ.py();
     double massT = w_et*w_et - w_px*w_px - w_py*w_py;
     massT = (massT>0) ? sqrt(massT) : 0;
 		
     hMETfromZmumu->Fill(metFromZ.pt()*(wMass/zMass));
     hMTfromZmumu->Fill(massT*(wMass/zMass)); 	

     tree_PtNu = neutrino->pt();
     tree_EtaNu = neutrino->eta();
     tree_VBPt = myZ.pt()*(wMass/zMass);
     tree_VBEta = myZ.eta();
     tree_VBPhi = myZ.phi();
     tree_METfromZmumu = metFromZ.pt()*(wMass/zMass);
     tree_MTfromZmumu = massT*(wMass/zMass);

     data->Fill();
   }  // End loop over events
  
  // Write output file
  hfile->cd();	
  hfile->Write();
  hfile->Close();

}

