//-*-c++-*-
//-*-HiggsExample.cpp-*-
//-*-Example ExHuME main program to generate Higgs-*-
//   Written by James Monk and Andrew Pilkington
/////////////////////////////////////////////////////////////////////////////
#include <cmath>

#include "DiPhoton.h"
#include "Event.h"
#include "PythiaRecord.h"

//#include "TFile.h"
//#include "TTree.h"

#define pylist pylist_

int main(int argc, char** argv){

  //TFile dat("GluGlu.root", "RECREATE");
  //TTree Data("Data", "Higgs");

  /*double m = 0.0;
  double y, t1, t2, x1, x2;
  int Nobj;
  double Hobj[2000][4];
  int Id[2000];
  double Proton1[4], Proton2[4];

  HepLorentzVector p1, p2;

  Data.Branch("Mass", &m, "m/D");
  Data.Branch("x1", &x1, "x1/D");
  Data.Branch("x2", &x2, "x2/D");
  Data.Branch("t1", &t1, "t1/D");
  Data.Branch("t2", &t2, "t2/D");
  Data.Branch("Rapidity", &y, "y/D");
  Data.Branch("Nobj", &Nobj, "Nobj/I");
  Data.Branch("Hobj", Hobj, "Hobj[Nobj][4]/D");
  Data.Branch("Id", &Id, "Id[Nobj]/I");
  Data.Branch("Proton1", Proton1, "Proton1[4]/D");
  Data.Branch("Proton2", Proton2, "Proton2[4]/D");*/
  
  Exhume::DiPhoton diphoton (argc, argv);
  Exhume::Event myEvent (diphoton, 1111);
  //                                ^^
  //                       Random number seed

  diphoton.SetThetaMin(0.30);
  myEvent.SetMassRange(20.0, 99999.0);
  myEvent.SetParameterSpace();

  std::cout<<"   Event generation Begins..."<<std::endl;

  int maxEventsToPrint = 2;
  int pythiaListVerbosity = 1;
  for(unsigned int i=0; i!= 300; i++){
    myEvent.Generate();
    diphoton.Hadronise();
    if(maxEventsToPrint){
       --maxEventsToPrint;
       pylist(pythiaListVerbosity);
    }

    /*m  = gluglu.GetSqrtsHat();
    x1 = gluglu.Getx1();
    x2 = gluglu.Getx2();
    t1 = gluglu.Gett1();
    t2 = gluglu.Gett2();
    y = gluglu.GetEta();

    Nobj = 0;

    for(int j=0;j<hepevt_.nhep; j++){
      if(hepevt_.isthep[j]==1){//Only store stable particles
	for(int k=0;k<4;k++){
	  Hobj[Nobj][k] = hepevt_.phep[j][k];
	}
	Id[Nobj] = hepevt_.idhep[j];
	Nobj++;
      }
    }

    p1 = gluglu.GetProton1();
    p2 = gluglu.GetProton2();

    Proton1[0] = p1.px();
    Proton1[1] = p1.py();
    Proton1[2] = p1.pz();
    Proton1[3] = p1.e();

    Proton2[0] = p2.px();
    Proton2[1] = p2.py();
    Proton2[2] = p2.pz();
    Proton2[3] = p2.e();

    Data.Fill();*/
  }

  double XS = myEvent.CrossSectionCalculation();
  double Eff = myEvent.GetEfficiency();
  std::string Name = diphoton.GetName();

  std::cout<<std::endl<<std::endl;
  std::cout<<"   You have just been ExHuMEd."<<std::endl<<std::endl;;
  std::cout<<"   The cross section for process "<<Name
	   <<" is "<<XS<<" fb"<<std::endl<<std::endl;
  std::cout<<"   The efficiency of event generation was "<<Eff<<"%"
	   <<std::endl<<std::endl;

  //Data.Write();

  return(0);
}
/////////////////////////////////////////////////////////////////////////////
