
/*
metod for the transport simulation
Scatt,Smea,Noise if true there will be the simulation of scattering,smearing and background
N1,N2 are the number of background signals for each primary vertex
*/
#include <Riostream.h>
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "Particle.h"
#include "Vertex.h"
#include "Layer.h"
#include "HitPoint.h"
#include "BeamPipe.h"
#include "TMath.h"
#include "TClonesArray.h"
#include "TStopwatch.h"
#include "MonteCarlo.h"
void MonteCarlo::MCTransport(const bool Scatt=false,const bool Smea=false,const bool Noise=true,const int N1=20,const int N2=20)
{
  if(fVerbose)
    cout<<"transport simulation ..."<<endl;
  TStopwatch timer;
  timer.Start();

  //smearing
  double SmeZ=120./10000;
  double SmeRP=30./10000;
  //beam pipe
  double R=3;//radius beampipe (cm)
  double S=0.8/10;
  double St=1./1000;
  //layer1
  double RL1=4;//radius layer1 (cm)
  double SL1=0.02;//thickness layer1 (cm)
  double l1=27;//lenght layer1 (cm)
  int NoiseL1=N1;
  double Efficiency1=1;
  //layer2
  double RL2=7;//radius layer2 (cm)
  double SL2=0.02;//thickness layer 2 (cm)
  double l2=27;//lenght layer2 (cm)
  int NoiseL2=N2;
  double Efficiency2=1;
  
  BeamPipe B(R,S,St);
  Layer L1(l1,RL1,SL2,SmeZ,SmeRP,NoiseL1,Efficiency1,St);
  Layer L2(l2,RL2,SL2,SmeZ,SmeRP,NoiseL2,Efficiency2,St);

  //input fil
  TFile hfile("DataFolder/MCdata.root");
  TTree *MCTree = (TTree*)hfile.Get("MCTree");
  TClonesArray *hits = new TClonesArray("Particle",100);
  TBranch *ParticleData=MCTree->GetBranch("Particle");
  ParticleData->SetAddress(&hits);

  //output file
  TFile sfile("DataFolder/MCtrasport.root","RECREATE");
  TTree *TraspTree = new TTree("TraspTree","");
  //Array for the signals
  //layer1 
  TClonesArray *ptrhits1 = new TClonesArray("HitPoint",100);
  TClonesArray& hits1 = *ptrhits1;
  TraspTree->Branch("HitPoint1",&ptrhits1);
  //layer2
  TClonesArray *ptrhits2 = new TClonesArray("HitPoint",100);
  TClonesArray& hits2 = *ptrhits2;
  TraspTree->Branch("HitPoint2",&ptrhits2);
  if(fVerbose)
    cout<<"event 0 on "<<MCTree->GetEntries()<<endl;
  
  for(int Event=0;Event<MCTree->GetEntries();Event++)
  {
    if(fVerbose && (Event+1)%5000==0)
      cout<<"event "<<Event+1<<" on "<<MCTree->GetEntries()<<endl;
    //to count the number of particles and add the background after 
    double NTrueData=0;

    MCTree->GetEvent(Event);
    int NumberOfParticles=hits->GetEntries();
    for (int j=0; j<NumberOfParticles; j++)
    {
      Particle *P=(Particle*)hits->At(j);
      
      //scattering simulation
      if(Scatt)
        B.Scattering(P);
      //creation of the signal and smearing simulation for Layer1
      //test to simulate the efficiency of revelation
      if(gRandom->Rndm()<L1.GetEfficiency())
      {
        
        new(hits1[j])HitPoint(L1,P);
        if(Scatt)
          L1.Scattering(P);
        //smearing simulation
        if(Smea)
          ((HitPoint*)hits1[j])->Smearing(L1);
      }
      else //if the signal is not taken
        new(hits1[j])HitPoint();

      //creation of the signal and smearing simulation for Layer2
      //test to simulate the efficiency of revelation
      if(gRandom->Rndm()<L2.GetEfficiency())
      {
        new(hits2[j])HitPoint(L2,P);
        if(Scatt)
          L2.Scattering(P);
        //smearing simulation
        if(Smea)
          ((HitPoint*)hits2[j])->Smearing(L2);
      }
      else //if the signal is not taken
        new(hits2[j])HitPoint();
      
      NTrueData++;
    }
    //background production
    if(Noise)
    {
      //layer1
      for(int n=0;n<L1.GetBackground();n++)
        new(hits1[n+NTrueData])HitPoint(L1);
      //layer2
      for(int n=0;n<L2.GetBackground();n++)
        new(hits2[n+NTrueData])HitPoint(L2);
    }
    TraspTree->Fill();
    ptrhits1->Clear();
    ptrhits2->Clear();
  }
  
  sfile.Write();
  sfile.Close();
  hfile.Close();
  if(fVerbose)
  {
    timer.Stop();
    cout<<"extecution time : ";
    timer.Print();
  }
}