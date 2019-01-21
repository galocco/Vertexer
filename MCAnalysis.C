/*
That is the function to analyze data from MCGenerator() and MCTransport()
dNoise is the pace of background included in the analysis 
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
using namespace std;
void MonteCarlo::MCAnalysis(const int dNoise=10)
{
  if(fVerbose)
    cout<<"Data analysis:"<<endl;
  TStopwatch timer;
  timer.Start();

  //opening of MCGenerator file
  static VertexData point;
  TFile hfile("DataFolder/MCdata.root");
  TTree *treeMC = (TTree*)hfile.Get("MCTree");
  TBranch *bMC=treeMC->GetBranch("MCTrueValue");
  bMC->SetAddress(&point);

  //opening of MCtransport file
  TFile sfile("DataFolder/MCtrasport.root");
  TTree *tree = (TTree*)sfile.Get("TraspTree");
  TClonesArray *hits1 = new TClonesArray("HitPoint",100);
  TClonesArray *hits2 = new TClonesArray("HitPoint",100);
  TBranch *b1=tree->GetBranch("HitPoint1");
  TBranch *b2=tree->GetBranch("HitPoint2");
  b1->SetAddress(&hits1);
  b2->SetAddress(&hits2);

  //definition of binning parameters for the primary vertex z measure histogram
  int UpEdge=25;
  int LowEdge=-25;
  int Binning=static_cast<int>((UpEdge-LowEdge)/fZ0BinWidth);

  //file where results will be saved
  TFile rfile("DataFolder/MCResults.root","RECREATE");
  //TNtupla for the primary vertex analysis saving 
  TNtuple *ZData = new TNtuple("ZData","data on vertex","Z:RMS:Z0:Multiplicity:NContr:Noise:Rec");
  

  if(fVerbose)
    cout<<"Evento 0 su "<<tree->GetEntries()<<endl;
  for(int Event=0;Event<tree->GetEntries();Event++)
  {
    tree->GetEvent(Event);
    treeMC->GetEvent(Event);
    if(fVerbose && (Event+1)%5000==0)
      cout<<"Evento "<<Event+1<<" su "<<tree->GetEntries()<<endl;
      
    int NumberOfSignals1=hits1->GetEntries();
    int NumberOfSignals2=hits2->GetEntries()
    //loop over the number of background signals, to simplify the study the both layers 
    //have the same level of background

    for(int Noise=0;Noise<=NumberOfSignals1-point.Multiplicity;Noise+=dNoise)
    {
      //histogram for the primary vertex z measure
      TH1D* HistoZ = new TH1D("HistoZ",";Z(cm);counts",Binning,LowEdge,UpEdge);

      //double loop to pair the signals 
      //vector to same all z measure to avoid loose of information
      vector<double> AllZs;
      for (int Singal1=0; Singal1<point.Multiplicity+Noise; Singal1++)
      {
        HitPoint *P1=(HitPoint*)hits1->At(Singal1);
        for (int Signal2=0; Signal2<point.Multiplicity+Noise; Signal2++)
        {
          HitPoint *P2=(HitPoint*)hits2->At(Signal2);
          //angle phi test
          bool GoodPhi=false;
          if(TMath::Abs(P1->GetPhi()-P2->GetPhi())<fDeltaPhi || TMath::Abs(TMath::Abs(P1->GetPhi()-P2->GetPhi())-2*TMath::Pi())<fDeltaPhi)
            GoodPhi=true;
          if(GoodPhi && P1->GetPhi()!=-1)
          { 
            //reconstruction of z 
            double z=ReconstructionZ(P1,P2);
            HistoZ->Fill(z);
            AllZs.push_back(z);
          }
        }
      }
      //HistoZ just to define the range where to calculate z

      double BinFZ=HistoZ->GetMaximumBin();
      double FZ=HistoZ->GetBinCenter(BinFZ);
      double MaxValue=HistoZ->GetBinContent(BinFZ);
      //ZRec will be the final estimation of primary vertex z
      double ZRec=0;
      double RMS=0;
      //number of zs used to calculate ZRec
      double contributors=0;
      for(auto Z:AllZs)
      {
        //ZRec is the mean of zs in the range
        if(Z>HistoZ->GetBinCenter(BinFZ-1) && Z<HistoZ->GetBinCenter(BinFZ+1))
        {
          contributors++;
          ZRec+=Z;
        }
      }
      ZRec/=contributors;
      for(auto Z:AllZs)
      {
        if(Z>HistoZ->GetBinCenter(BinFZ-1) && Z<HistoZ->GetBinCenter(BinFZ+1))
          RMS+=(Z-ZRec)*(Z-ZRec);
      }
      RMS=TMath::Sqrt(RMS/(contributors));
      bool MaxIsUnique=true;
      for(int Bin=1;Bin<=HistoZ->GetNbinsX();Bin++)
      {
        if(FZ==HistoZ->GetBinContent(Bin) && (Bin<BinFZ-1 || Bin>BinFZ+1))
          MaxIsUnique=false;
      }
      /*
      the ZRec is considered reconstructed if in the histogram there is at least one entry
      and the max is unique 
      */
      if(HistoZ->GetEntries()!=0 && MaxIsUnique)
        ZData->Fill(ZRec,RMS,point.Z0,point.Multiplicity,contributors,Noise,1);
      else      
        ZData->Fill(ZRec,RMS,point.Z0,point.Multiplicity,contributors,Noise,0);

      HistoZ->Delete();
    }
  }

  rfile.cd();
  rfile.Write();
  rfile.Close();
  hfile.Close();
  sfile.Close();
  if(fVerbose)
  {
    cout<<"execution time : ";
    timer.Stop();
    timer.Print();
  }
}