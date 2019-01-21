/*
metod to set fZ0binWidth

NumberOfVertices is the number of primary vertices used
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
#include "TStopwatch.h"
#include "MonteCarlo.h"
//
void MonteCarlo::DeltaZAnalysis(const int NumberOfVertices=10000)
{
  if(fVerbose)
    cout<<"fZ0BinWidth setting ..."<<endl;
  TStopwatch timer;
  timer.Start();

  //generation file
  static VertexData point;
  TFile hfile("DataFolder/MCdata.root");
  TTree *treeMC = (TTree*)hfile.Get("MCTree");
  TBranch *bMC=treeMC->GetBranch("MCTrueValue");
  bMC->SetAddress(&point);

  //transport file
  TFile sfile("DataFolder/MCtrasport.root");
  TTree *tree = (TTree*)sfile.Get("TraspTree");
  TClonesArray *hits1 = new TClonesArray("HitPoint",100);
  TClonesArray *hits2 = new TClonesArray("HitPoint",100);
  TBranch *b1=tree->GetBranch("HitPoint1");
  TBranch *b2=tree->GetBranch("HitPoint2");
  b1->SetAddress(&hits1);
  b2->SetAddress(&hits2);

  if(fVerbose)
    cout<<"event 0 on "<<NumberOfVertices<<endl;
  //output file
  TFile rfile("DataFolder/MCZrec.root","RECREATE");
  TH1D* HistoRecDistr = new TH1D("HistoRecDistr",";#DeltaZ(cm);counts",100,-0.3,0.3);
  for(int Event=0;Event<=NumberOfVertices;Event++)
  {
    if(fVerbose && (Event+1)%5000==0)
      cout<<"event "<<Event+1<<" on "<<NumberOfVertices<<endl;
    
    tree->GetEvent(Event);
    treeMC->GetEvent(Event);
    //this time just one loop because I am only interested in study true signals
    for (int j=0; j<point.Multiplicity; j++)
    {t
      HitPoint *P1=(HitPoint*)hits1->At(j);
      HitPoint *P2=(HitPoint*)hits2->At(j);
      //if the particles is gone through the detectors
      if(P1->GetPhi()!=-1)
      { 
        //reconstruction of primary vertex z
        double z=ReconstructionZ(P1,P2);
        HistoRecDistr->Fill(z-point.Z0);
      }
    }
  }

  /*
  z-z0 have gaussian distribution, six times the RMS will include 99% of the zs
  */
  fZ0BinWidth=HistoRecDistr->GetRMS()*6;

  rfile.cd();
  rfile.Write();
  rfile.Close();
  hfile.Close();
  sfile.Close();

  timer.Stop();
  cout<<"execution time : ";
  timer.Print();
}