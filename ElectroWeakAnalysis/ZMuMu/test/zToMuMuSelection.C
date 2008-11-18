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
static const double zMass = 91.1876; // PDG Z mass
void zToMuMuSelection(int maxEvents = -1, bool verbose = false) {

   // Create a vector of input files
   vector<string> fileNames;
   //fileNames.push_back("rfio:/castor/cern.ch/user/a/antoniov/Analysis/zToMuMu/zToMuMuNew_ReReco10pb.root");
   //fileNames.push_back("/tmp/antoniov/zToMuMu_1k.root");
   fileNames.push_back("/tmp/antoniov/zToMuMuNew_ReReco10pb.root");
   
   // Chain the input files
   fwlite::ChainEvent ev(fileNames);
   
   // Create output file
   TFile* hfile = new TFile("histos.root","recreate","data histograms");

   // Book Histograms
   TH1F* hZmass = new TH1F("hZmass","hZmass",100, 60., 120.);
   TH1F* hZpt = new TH1F("hZpt","hZpt",100, 0., 40.);
   TH1F* hZphi = new TH1F("hZphi","hZphi",100, -3.141593, 3.141593);
   TH1F* hZeta = new TH1F("hZeta","hZeta",100, -5.0, 5.0);

   TH1F* hMET = new TH1F("hMET","hMET",100, 0., 40.);
   TH1F* hMETZParallel = new TH1F("hMETZParallel","hMETZParallel",100, -50., 50.);
   TH2F* hMETvsZpt = new TH2F("hMETvsZpt","hMETvsZpt",100, 0., 100.,100, 0., 100.);

   TH1F* hCaloMET = new TH1F("hCaloMET","hCaloMET",100, 0., 40.);
   TH1F* hCaloMETParal = new TH1F("hCaloMETParal","hCaloMETParal",100, -40., 60.);
   TH1F* hCaloMETPerp = new TH1F("hCaloMETPerp","hCaloMETPerp",100, -40., 40.);
   TH1F* hCaloMETPhi = new TH1F("hCaloMETPhi","hCaloMETPhi",100, -3.141593, 3.141593);

   TH1F* hCaloMETPhiminusZphi = new TH1F("hCaloMETPhiminusZphi","hCaloMETPhiminusZphi",100, -3.141593, 3.141593);
   TH2F* hCaloMETvsZpt = new TH2F("hCaloMETvsZpt","hCaloMETvsZpt",100, 0., 100.,100, 0., 100.);
   TH2F* hCaloMETParalvsZpt = new TH2F("hCaloMETParalvsZpt","hCaloMETParalvsZpt",100, 0., 60.,100, -40., 60.);
   TH2F* hCaloMETPerpvsZpt = new TH2F("hCaloMETPerpvsZpt","hCaloMETPerpvsZpt",100, 0., 60.,100, -40., 40.);	

   // Z Selection cuts
   double ptCut_ = 20.0;
   double etaCut_ = 2.0;
   float isoCut_ = 3.0;
   double minZmass = 70.0;
   double maxZmass = 100.0; 			
   // Loop over the events
   int nEvts = 0;
   for( ev.toBegin(); ! ev.atEnd(); ++ev) {

     if(nEvts == maxEvents) break;	
     // Get the collections
     if(verbose) std::cout << ">>> Event number: " << ++nEvts << endl;	
     // Z candidates
     fwlite::Handle<reco::CompositeCandidateCollection> zMuMu; 
     zMuMu.getByLabel(ev,"goodZToMuMu");

     // Corrected MET	
     fwlite::Handle<reco::CaloMETCollection> metColl;
     metColl.getByLabel(ev,"corMetType1Icone5");
	
     const reco::CaloMET& met = metColl->front();

     // Isolations
     fwlite::Handle<edm::ValueMap<float> > muonIso;
     muonIso.getByLabel(ev,"muonIsolations");
	
     const edm::ValueMap<float>& muIsolations = *muonIso;

     //now can access data
     //std::cout <<" size "<<objs.ptr()->size()<<std::endl;

     /*for (unsigned int iter = 0; iter < objs.ptr()->size(); ++iter) {
       reco::Muon muon=(*objs.ptr())[iter];
       cout << "Muon Pt=" <<  muon.pt() << endl;
     } 
     cout << "  " << endl; */

     double CaloMET_Ex = met.px();
     double CaloMET_Ey = met.py();			

     // Do Z selection
     bool eventSelected = false;

     //std::cout << "Number of Z candidates: " << zMuMu->size() << endl;	
     if(zMuMu->size() < 1) continue;	

     if(verbose) std::cout << "beginning z loop" << std::endl;
     int iselZ = -1;
     double aux_mass = 999999.;		      
     for(size_t iter = 0; iter < zMuMu->size(); ++iter){
	const reco::Candidate& zmmCand = (*zMuMu)[iter];
	reco::MuonRef muon1 = zmmCand.daughter(0)->masterClone().castTo<reco::MuonRef>();
	reco::MuonRef muon2 = zmmCand.daughter(1)->masterClone().castTo<reco::MuonRef>();
	
	float iso1 = muIsolations[muon1];
        float iso2 = muIsolations[muon2];
	if(verbose) std::cout << "muon 1 pt,eta,iso= " << muon1->pt() << " , " << muon1->eta() << " , " << iso1 << std::endl;
	if(verbose) std::cout << "muon 2 pt,eta,iso= " << muon2->pt() << " , " << muon2->eta() << " , " << iso2 << std::endl;

	/*reco::CandidateBaseRef muRef1(muon1);
	reco::CandidateBaseRef muRef2(muon2);
	float iso1 = muIsolations[muRef1];
	float iso2 = muIsolations[muRef2];*/

	if(((muon1->pt() > ptCut_)&&(fabs(muon1->eta()) < etaCut_)&&(iso1 < isoCut_))&&
	   ((muon2->pt() > ptCut_)&&(fabs(muon2->eta()) < etaCut_)&&(iso2 < isoCut_))&&
	   (zmmCand.mass() > minZmass)&&(zmmCand.mass() < maxZmass)){
		eventSelected = true;
		if(fabs(zmmCand.mass() - zMass) < fabs(aux_mass - zMass)){
			aux_mass = zmmCand.mass();
			iselZ = iter;
		} 
	}
	//hZmass->Fill(zmmCand.mass());	
     }
     if(verbose) std::cout << "after z loop" << std::endl;
     if(!eventSelected) continue;
     assert(iselZ >= 0);	
	
     const reco::Candidate& myZ = (*zMuMu)[iselZ];
     assert(myZ.numberOfDaughters() == 2);
     const reco::Candidate* mu1 = myZ.daughter(0);
     const reco::Candidate* mu2 = myZ.daughter(1);
     if((mu1 == 0)||(mu2 == 0)) continue;

     CaloMET_Ex += mu1->px();
     CaloMET_Ex += mu2->px();

     CaloMET_Ey += mu1->py();
     CaloMET_Ey += mu2->py();		

     double CaloMET_Et = sqrt(CaloMET_Ex*CaloMET_Ex + CaloMET_Ey*CaloMET_Ey);
     math::XYZTLorentzVector myCaloMET(CaloMET_Ex,CaloMET_Ey,0.,CaloMET_Et);

     // Get parallel and perpendicular components of MET (corrected)
     double MET_paral = (met.px()*myZ.px() + met.py()*myZ.py())/myZ.pt();
 
     //double CaloMET_Paral = (myCaloMET.px()*myZ.px() + myCaloMET.py()*myZ.py())/myZ.pt();	    	
     //double CaloMET_Perp = sqrt(CaloMET_Et*CaloMET_Et - CaloMET_Paral*CaloMET_Paral);

     // Change coordinates to the Z in the x^{prime} axis
     double cosZ = myZ.px()/myZ.pt();
     double sinZ = myZ.py()/myZ.pt();	
     double CaloMET_Paral = myCaloMET.px()*cosZ + myCaloMET.py()*sinZ;
     double CaloMET_Perp = -myCaloMET.px()*sinZ + myCaloMET.py()*cosZ;	     
	
     hZmass->Fill(myZ.mass());	
     hZpt->Fill(myZ.pt());
     hZphi->Fill(myZ.phi());
     hZeta->Fill(myZ.eta());

     hMET->Fill(met.pt());
     hMETZParallel->Fill(MET_paral);	
     hMETvsZpt->Fill(myZ.pt(),met.pt());

     hCaloMET->Fill(CaloMET_Et);
     hCaloMETParal->Fill(CaloMET_Paral);
     hCaloMETPerp->Fill(CaloMET_Perp);
     hCaloMETPhi->Fill(myCaloMET.phi());

     hCaloMETvsZpt->Fill(myZ.pt(),CaloMET_Et);
     hCaloMETParalvsZpt->Fill(myZ.pt(),CaloMET_Paral);
     hCaloMETPerpvsZpt->Fill(myZ.pt(),CaloMET_Perp);
     hCaloMETPhiminusZphi->Fill(myCaloMET.phi() - myZ.phi());
   }  // End loop over events
  
  // Write output file
  hfile->cd();	
  hfile->Write();
  hfile->Close();

}

