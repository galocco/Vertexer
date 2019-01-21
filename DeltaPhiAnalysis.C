/*
macro to define the optimal angle tollerance to pair two signal
PhiIn,e PhiFin define the angle tollerance.
dPhi is the pace [rad].
NumberOfVertices is the number of events used,

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
#include "TNtuple.h"
#include "TClonesArray.h"
#include "MonteCarlo.h"
#include "TStopwatch.h"

using namespace std;
void MonteCarlo::DeltaPhiAnalysis(const double PhiIn=0.0001,const double PhiFin=0.01,const double dPhi=0.0001,const int NumberOfVertices=1000)
{
  if(fVerbose)
    cout<<"DeltaPhi analysis  ..."<<endl;
  TStopwatch timer;
  timer.Start();
  //MonteCarlo data
  static VertexData point;
  TFile hfile("DataFolder/MCdata.root");
  TTree *treeMC = (TTree*)hfile.Get("MCTree");
  TBranch *bMC=treeMC->GetBranch("MCTrueValue");
  bMC->SetAddress(&point);
  //Transported data
  TFile sfile("DataFolder/MCtrasport.root");
  TTree *tree = (TTree*)sfile.Get("TraspTree");
  TClonesArray *hits1 = new TClonesArray("HitPoint",100);
  TClonesArray *hits2 = new TClonesArray("HitPoint",100);
  TBranch *b1=tree->GetBranch("HitPoint1");
  TBranch *b2=tree->GetBranch("HitPoint2");
  b1->SetAddress(&hits1);
  b2->SetAddress(&hits2);
  //Saving file
  TFile R("DataFolder/MCPhi.root","RECREATE");
  TH1D *HistoEff = new TH1D("HistoEff",";#Delta#phi (rad);Reconstructed/Generated",(PhiFin-PhiIn)/dPhi+1,PhiIn,PhiFin);
  TH1D *HistoRum = new TH1D("HistoRum",";#Delta#phi (rad);True tracks/Total tracks",(PhiFin-PhiIn)/dPhi+1,PhiIn,PhiFin);
  TH1D *HistoProd = new TH1D("HistoProd",";#Delta#phi (rad);(True tracks)^2/(Total tracks x Particle generated) ",(PhiFin-PhiIn)/dPhi+1,PhiIn,PhiFin);
  TH1D *HistoTotTracks = new TH1D("HistoTotTracks",";#Delta#phi (rad); Total tracks",(PhiFin-PhiIn)/dPhi+1,PhiIn,PhiFin);
  


  if(fVerbose)
    cout<<"Evento 0 su "<<NumberOfVertices<<endl;
  //number of particle through the revelator
  int TrueTracks=0;
  for(int Event=0;Event<=NumberOfVertices;Event++)
  {
    if(fVerbose && (Event+1)%1000==0)
      cout<<"Evento "<<Event+1<<" su "<<NumberOfVertices<<endl;
    
    tree->GetEvent(Event);
    treeMC->GetEvent(Event);
    int NumberOfParticles=hits1->GetEntries();
    for (int j=0; j<NumberOfParticles; j++)
    {
      HitPoint *P1=(HitPoint*)hits1->At(j);
      for (int k=0; k<NumberOfParticles; k++)
      {
        HitPoint *P2=(HitPoint*)hits2->At(k);
        //add a particle if it is gone through the detectors
        if(k==j && P1->GetR()!=0 && P2->GetR()!=0 && k<point.Multiplicity && j<point.Multiplicity)
          TrueTracks++;
        //loop to pair changing the tollerance
        for(double DeltaPhi=PhiIn;DeltaPhi<=PhiFin;DeltaPhi+=dPhi)
        {
          //angle test
          bool GoodPhi=false;
          if(TMath::Abs(P1->GetPhi()-P2->GetPhi())<DeltaPhi || 2*TMath::Pi()-TMath::Abs(P1->GetPhi()-P2->GetPhi())<DeltaPhi)
            GoodPhi=true;
          //if there is a pairing
          if(GoodPhi && P1->GetR()!=0 && P2->GetR()!=0)
          { 
            HistoTotTracks->Fill(DeltaPhi);
            //if is a real particle
            if(j==k && k<point.Multiplicity && j<point.Multiplicity)
            {
              HistoEff->Fill(DeltaPhi);
              HistoRum->Fill(DeltaPhi);
            }
          }
        }
      }
    }
  }
  
  for(int Bin=1;Bin<=HistoEff->GetNbinsX();Bin++)
  {
    double Eff=HistoEff->GetBinContent(Bin);
    double Rum=HistoRum->GetBinContent(Bin);
    double TotTracks=HistoTotTracks->GetBinContent(Bin);
    //fraction of the pairing in the particle
    HistoEff->SetBinContent(Bin,Eff/TrueTracks);
    //fraction of true pairings
    HistoRum->SetBinContent(Bin,Eff/TotTracks);
    Eff=HistoEff->GetBinContent(Bin);
    Rum=HistoRum->GetBinContent(Bin);
    HistoProd->SetBinContent(Bin,Eff*Rum);
    //setting of the uncertainties
    HistoEff->SetBinError(Bin,TMath::Sqrt(Eff*(1-Eff)/TrueTracks));
    HistoRum->SetBinError(Bin,TMath::Sqrt(Rum*(1-Rum)/TotTracks));
    HistoProd->SetBinError(Bin,TMath::Sqrt(Eff*(1-Eff)/TrueTracks)+Rum*(1-Rum)/TotTracks);
  }
  //scelgo la distanza massima in phi come il centro del primo bin ad associatere tutte le tracce vere
  for(int Bin=1;Bin<=HistoEff->GetNbinsX();Bin++)
  {
    if(HistoEff->GetBinContent(Bin)==1)
    {
      fDeltaPhi=HistoEff->GetBinCenter(Bin);
      break;
    }
  }
  R.Write();
  hfile.Close();
  sfile.Close();
  R.Close();

  if(fVerbose)
  {
    timer.Stop();
    cout<<"execution time : ";
    timer.Print();
  }
}