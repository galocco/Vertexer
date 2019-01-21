#include "Particle.h"
#include "TObject.h"
ClassImp(Particle)
Particle::Particle():TObject()
{
  fPhi=0;
  fTheta=0;
  fC[0]= 0;
  fC[1]= 0;
  fC[2]= 0;
  fX[0]= 0;
  fX[1]= 0;
  fX[2]= 0;
}
/*
the constructor set fX[] equal to the position of the primary vertex V and eta(then fC[])
from HistoDistribution in the range [EtaMin,EtaMax]
*/
Particle::Particle(Vertex V,TH1D* HistoDistribution,double EtaMin,double EtaMax):TObject()
{
  HistoDistribution->SetDirectory(0);
  HistoDistribution->SetMinimum(0);
  TAxis *xa=HistoDistribution->GetXaxis();
  double step = xa->GetBinWidth(1);
  int b1=xa->FindBin(EtaMin);
  int b2=xa->FindBin(EtaMax);
  double xlow=xa->GetBinLowEdge(b1);
  double xhig=xa->GetBinUpEdge(b2);
  int nobins=b2-b1+1;
  double step2 = (xhig-xlow)/nobins;
  TH1F* histoEta = new TH1F("histoEta","",nobins,xlow,xhig);
  int j=1;
  for(int i=b1;i<=b2;i++)
    histoEta->SetBinContent(j++,HistoDistribution->GetBinContent(i));

  fTheta = 2*TMath::ATan(TMath::Exp(-histoEta->GetRandom()));
  fPhi = gRandom->Rndm()*2*TMath::Pi();
  fC[0] = TMath::Sin(fTheta)*TMath::Cos(fPhi);
  fC[1] = TMath::Sin(fTheta)*TMath::Sin(fPhi);
  fC[2] = TMath::Cos(fTheta);
  fX[0]=V.GetP0(0);
  fX[1]=V.GetP0(1);
  fX[2]=V.GetP0(2);
  histoEta->Delete();
}
Particle::Particle(double p0[],double theta,double phi):TObject()
{
  fTheta=theta;
  fPhi=phi;
  for(int c=0;c<=2;c++)
    fX[c] = p0[c];
  fC[0] = TMath::Sin(fTheta)*TMath::Cos(fPhi);
  fC[1] = TMath::Sin(fTheta)*TMath::Sin(fPhi);
  fC[2] = TMath::Cos(fTheta);
}
Particle::~Particle()
{
}
//copy constructor
Particle& Particle::operator=(const Particle& source)
{
  if(this!=&source)
  {
    TObject::operator=(source);
    fTheta=source.fTheta;
    fPhi=source.fPhi;
    for(int i=0;i<3;i++)
    {
      fX[i]=source.fX[i];
      fC[i]=source.fC[i];
    }
  }
  return *this;
}
