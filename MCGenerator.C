/*
metod for the events generation
*/
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TMath.h"
#include "TClonesArray.h"
#include "Riostream.h"
#include "TRandom3.h"
#include "Vertex.h"
#include "Particle.h"
#include "TStopwatch.h"
#include "MonteCarlo.h"

void MonteCarlo::MCGenerator()
{
  if(fVerbose)
    cout<<"generation of the primary vertices ..."<<endl;
  TStopwatch timer;
  timer.Start();

  gRandom->SetSeed(fSeed);
  //dispersion from the axis origin (0,0,0)
  double Sz=5.3;
  double Sx=0.01;
  double Sy=0.01;
  //struct for primary vertex data
  VertexData point;
  //opening of true distribution file
  TFile F("DataFolder/kinem.root");
  //true distribution
  TH1D* RealMultiplicityDistribution= (TH1D*)F.Get("hmul");
  TH1D* EtaDistribution= (TH1D*)F.Get("heta");
  //constant distribution
  TH1D* ConstMultiplicity= new TH1D("ConstMultiplicity","",50,0.5,50.5);
  for(int Bin=1;Bin<=50;Bin++)
    ConstMultiplicity->SetBinContent(Bin,1);

  //file where events will be saved
  TFile hfile("DataFolder/MCdata.root","RECREATE");
  TTree *tree = new TTree("MCTree","");
  TClonesArray *ptrhits = new TClonesArray("Particle",100);
  TClonesArray& hits = *ptrhits;
  //branch for the vertices data
  tree->Branch("MCTrueValue",&point.Multiplicity,"Multiplicity/D:Z0");
  //branch for the particles
  tree->Branch("Particle",&ptrhits);

  TH1D* MultiplicityDistribution;
  if(fRealDistribution)
    MultiplicityDistribution=RealMultiplicityDistribution;
  else
    MultiplicityDistribution=ConstMultiplicity;
   //every loop simulate a collision   
  if(fVerbose)
    cout<<"events 0 on "<<fPrimaryVertices<<endl;
  for(int Event=0; Event<fPrimaryVertices;Event++)
  {
    if(fVerbose && (Event+1)%5000==0)
      cout<<"events "<<Event+1<<" on "<<fPrimaryVertices<<endl;
    
    //creation of the primary vertex
    Vertex V(Sx,Sy,Sz,MultiplicityDistribution);
    point.Multiplicity=V.GetMultiplicity();
    point.Z0=V.GetP0(2);
    //creation of particle in the tree
    for(int Ev=0;Ev<V.GetMultiplicity();Ev++)
      new(hits[Ev])Particle(V,EtaDistribution,-fEta,fEta);
    tree->Fill();
    
    ptrhits->Clear();
  }

  hfile.Write();
  F.Close();
  hfile.Close();
  if(fVerbose)
  {
    timer.Stop();
    cout<<"execution time :";
    timer.Print();
  }
}







