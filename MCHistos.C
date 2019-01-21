/*
metod to create histograms
projection = true will produce also the histograms of efficiency and resolution
dNoise is a binning parameter for the study of efficiency and resolution at different 
levels of background
*/
#include <Riostream.h>
#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TMath.h"
#include "TNtuple.h"
#include "TClonesArray.h"
#include "MonteCarlo.h"
void MonteCarlo::MCHistos(const bool projection=true,const int dNoise=10)
{
  cout<<"Creazione del file degli istogrammi ..."<<endl;
  float Z,RMS,Z0,Multiplicity,Contributor,Noise,Rec;
  TFile sfile("DataFolder/MCResults.root"); 
  TNtuple *ZData = (TNtuple*)sfile.Get("ZData");
  ZData->SetBranchAddress("Z",&Z);
  ZData->SetBranchAddress("RMS",&RMS);
  ZData->SetBranchAddress("Z0",&Z0);
  ZData->SetBranchAddress("Multiplicity",&Multiplicity);
  ZData->SetBranchAddress("NContr",&Contributor);
  ZData->SetBranchAddress("Noise",&Noise);
  ZData->SetBranchAddress("Rec",&Rec);

  int NoiseMax=0;
  for(int Event=0;Event<ZData->GetEntries();Event++)
  {
    ZData->GetEvent(Event);
    if(NoiseMax<Noise)
      NoiseMax=Noise;
  }
  int NBinNoise=NoiseMax/dNoise+1;
  int BinIn=-dNoise/2;
  int BinFin=NoiseMax+dNoise/2;

  TFile HistoFile("DataFolder/MCHistograms.root","RECREATE");
  TH2D* HistoZAll = new TH2D("HistoZAll",";Z(cm);Noise",50,-20,20,NBinNoise,BinIn,BinFin);
  
  TH2D* HistoAllMultivsNoise = new TH2D("HistoAllMultivsNoise",";;",50,0.5,50.5,NBinNoise,BinIn,BinFin);
  TH2D* HistoEffMultivsNoise = new TH2D("HistoEffMultivsNoise",";Multiplicity;Noise",50,0.5,50.5,NBinNoise,BinIn,BinFin);
  TH1D* HistoProjEffMulti; 

  TH2D* HistoAllZgenvsNoise = new TH2D("HistoAllZgenvsNoise",";;",30,-20,20,NBinNoise,BinIn,BinFin);
  TH2D* HistoEffZgenvsNoise = new TH2D("HistoEffZgenvsNoise",";Zgen(cm);Noise",30,-20,20,NBinNoise,BinIn,BinFin);
  TH1D* HistoProjEffZgen;


  TH3D* HistoDeltaZvsMultivsNoise= new TH3D("HistoDeltaZvsMultivsNoise",";Multiplicity;#DeltaZ(cm);Noise",50,0.5,50.5,50,-0.1,0.1,NBinNoise,BinIn,BinFin);
  TH2D* HistoRisMultivsNoise = new TH2D("HistoRisMultivsNoise",";Multiplicity;Noise",50,0.5,50.5,NBinNoise,BinIn,BinFin);
  TH1D* HistoProjRisMulti;

  TH3D* HistoDeltaZvsContributorsvsNoise= new TH3D("HistoDeltaZvsContributorsvsNoise",";Contributors;#DeltaZ(cm);Noise",50,0.5,50.5,50,-0.1,0.1,NBinNoise,BinIn,BinFin);
  TH2D* HistoRisContributorsvsNoise = new TH2D("HistoRisContributorsvsNoise",";Contributors;Noise",50,0.5,50.5,NBinNoise,BinIn,BinFin);
  TH1D* HistoProjRisContributors;

  TH3D* HistoDeltaZvsZgenvsNoise= new TH3D("HistoDeltaZvsZgenvsNoise",";Z_{gen}(cm);#DeltaZ(cm);Noise",40,-20,20,50,-0.1,0.1,NBinNoise,BinIn,BinFin);
  TH2D* HistoRisZgenvsNoise = new TH2D("HistoRisZgenvsNoise",";Z_{gen}(cm);Noise",40,-20,20,NBinNoise,BinIn,BinFin);
  TH1D* HistoProjRisZgen;
  TH3D* HistoDeltaZvsContributorsvsZ= new TH3D("HistoDeltaZvsContributorsvsZ",";Contributors;#DeltaZ(cm);Z(cm)}",50,0.5,50.5,50,-0.1,0.1,50,-15,15);
  TH2D* HistoRisolutionContributorsvsZ = new TH2D("HistoRisolutionContributorsvsZ",";Contributors;Z(cm)",50,0.5,50.5,50,-15,15);
    
  TH1D* HistoProj;
  
  for(int Event=0;Event<ZData->GetEntries();Event++)
  {

    ZData->GetEvent(Event);
    if(Rec>0.5)
    {
      HistoZAll->Fill(Z,Noise);
      HistoEffMultivsNoise->Fill(Multiplicity,Noise);
      HistoEffZgenvsNoise->Fill(Z0,Noise);
      HistoDeltaZvsMultivsNoise->Fill(Multiplicity,Z-Z0,Noise);
      HistoDeltaZvsContributorsvsNoise->Fill(Contributor,Z-Z0,Noise);
      HistoDeltaZvsZgenvsNoise->Fill(Z0,Z-Z0,Noise);
      HistoDeltaZvsContributorsvsZ->Fill(Contributor,Z-Z0,Z);
    }
    HistoAllMultivsNoise->Fill(Multiplicity,Noise);
    HistoAllZgenvsNoise->Fill(Z0,Noise);

  }

  ZData->Delete();
  sfile.Close();
  if(projection)
  {
    EfficiencyTH2(HistoEffMultivsNoise,HistoAllMultivsNoise,HistoProjEffMulti,dNoise,"Multiplicity");
    EfficiencyTH2(HistoEffZgenvsNoise,HistoAllZgenvsNoise,HistoProjEffZgen,dNoise,"Zgen");
    RisolutionTH3(HistoDeltaZvsMultivsNoise,HistoRisMultivsNoise,HistoProjRisMulti,dNoise,"Multiplicity");
    RisolutionTH3(HistoDeltaZvsContributorsvsNoise,HistoRisContributorsvsNoise,HistoProjRisContributors,dNoise,"Contributors");
    RisolutionTH3(HistoDeltaZvsZgenvsNoise,HistoRisZgenvsNoise,HistoProjRisZgen,dNoise,"Zgen");
  }
  
  for(int BinX=1;BinX<HistoDeltaZvsContributorsvsZ->GetNbinsX();BinX++)
  {
    for(int BinZ=1;BinZ<HistoDeltaZvsContributorsvsZ->GetNbinsZ();BinZ++)
    {
      HistoProj=(TH1D*)HistoDeltaZvsContributorsvsZ->ProjectionY("",BinX,BinX,BinZ,BinZ);
      double RMS=HistoProj->GetRMS();
      double ErrRMS=HistoProj->GetRMSError();
      HistoRisolutionContributorsvsZ->SetBinContent(BinX,BinZ,RMS);
      HistoRisolutionContributorsvsZ->SetBinError(BinX,BinZ,ErrRMS);
      HistoProj->Delete();
    }
  }
  
  HistoFile.Write();
  HistoFile.Close();
}
void MonteCarlo::RisolutionTH3(TH3D* Histo3,TH2D* Histo2,TH1* Histo1,int dNoise,char Name[])
{
  TH1D* HistoProj; 

  for(int BinX=1;BinX<=Histo3->GetNbinsX();BinX++)
  {
    for(int BinZ=1;BinZ<=Histo3->GetNbinsZ();BinZ++)
    {
      HistoProj=(TH1D*)Histo3->ProjectionY("",BinX,BinX,BinZ,BinZ);
      double RMS=HistoProj->GetRMS();
      double ErrRMS=HistoProj->GetRMSError();
      Histo2->SetBinContent(BinX,BinZ,RMS);
      Histo2->SetBinError(BinX,BinZ,ErrRMS);
      HistoProj->Delete();
    }
  }
  for(int bin=1;bin<=Histo2->GetNbinsY();bin++)
  {
    Histo1=(TH1D*)Histo2->ProjectionX(Form("Risolution vs %s with background = %i",Name,(bin-1)*dNoise),bin,bin);
    Histo1->GetYaxis()->SetTitle("Risolution (cm)");
  }
}

void MonteCarlo::EfficiencyTH2(TH2D* Histo2Ric,TH2D* Histo2Tot,TH1D* Histo1,int dNoise,char Name[])
{
  Histo2Ric->Divide(Histo2Tot);
  for(int bin=1;bin<=Histo2Ric->GetNbinsY();bin++)
  {
    TH1D* HistoProj;
    Histo1=(TH1D*)Histo2Ric->ProjectionX(Form("Efficiency vs %s with background = %i",Name,(bin-1)*dNoise),bin,bin);
    HistoProj=(TH1D*)Histo2Tot->ProjectionX(Form("AllZgen with background = %i",(bin-1)*dNoise),bin,bin);
    Histo1->GetYaxis()->SetTitle("Efficiency");
    for(int Bin=1;Bin<=Histo1->GetNbinsX();Bin++)
    {
      double Eff=Histo1->GetBinContent(Bin);
      double Tot=HistoProj->GetBinContent(Bin);
      Histo1->SetBinError(Bin,TMath::Sqrt(Eff*(1-Eff)/Tot));
    }
    HistoProj->Delete();
  }
}