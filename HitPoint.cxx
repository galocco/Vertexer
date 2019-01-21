#include "HitPoint.h"
#include "TObject.h"
#include "Layer.h"
ClassImp(HitPoint)
//default to values for no signal
HitPoint::HitPoint():TObject()
{
  fPhi=-1;
  fR=0;
  fZ=-100;
}

HitPoint::HitPoint(double Z,double Phi,double R):TObject()
{
  fPhi=Phi;
  fR=R;
  fZ=Z;
}
//constructor for the signal given by a detector
HitPoint::HitPoint(Layer L,Particle *P):TObject()
{
  double x[3];
  double c[3];
  for(int i=0;i<=2;i++)
  {
    x[i]=P->GetP0(i);
    c[i]=P->GetDirection(i);
  }
  //calculation of t parameter in the intersection
  //solving {xi=fX[i]+fC[i],x1^2+x2^2=fR}
  double delta = (x[0]*c[0]+x[1]*c[1])*(x[0]*c[0]+x[1]*c[1])-(c[0]*c[0]+c[1]*c[1])*(x[0]*x[0]+x[1]*x[1]-L.GetRadius()*L.GetRadius());
  double t = (-(x[0]*c[0]+x[1]*c[1])+TMath::Sqrt(delta))/(c[0]*c[0]+c[1]*c[1]);
  double xh=x[1]+c[1]*t;
  double yh=x[0]+c[0]*t;
  double tg=yh/xh;
  //if the particle go through the detector
  if(TMath::Abs(x[2]+c[2]*t)<=L.GetLenght()/2.)
  {
    fR=L.GetRadius();
    fZ=x[2]+c[2]*t;
    if(xh==0)
    {
      if(yh>0)
        fPhi=TMath::Pi();
      else
        fPhi=TMath::Pi()*3/2;
    }
    else if(xh>0)
    {
      if(yh>0)
        fPhi=TMath::ATan(tg);
      else
        fPhi=2*TMath::Pi()+TMath::ATan(tg);
    }
    else
    {
        fPhi=TMath::Pi()+TMath::ATan(tg);
    }
  }
  else
  {
    //values for no signal
    fR=0;
    fZ=-100;
    fPhi=-1;
  }
}
//constructor for the background
HitPoint::HitPoint(Layer L)
{
  fPhi=2*TMath::Pi()*gRandom->Rndm();
  fZ=(gRandom->Rndm()-0.5)*L.GetLenght();
  fR=L.GetRadius();
}

//simulation of the smearing
void HitPoint::Smearing(Layer L)
{ 
  //if there no signal
  if(fZ==-100)
    return;
  fZ+=gRandom->Gaus(0,L.GetSmearingZ());
  fPhi+=gRandom->Gaus(0,L.GetSmearingPhiR())/L.GetRadius();
}
HitPoint::~HitPoint()
{
}
//copy constructor 
HitPoint& HitPoint::operator=(const HitPoint& source)
{
  if(this!=&source)
  {
    TObject::operator=(source);
    fPhi=source.fPhi;
    fR=source.fR;
    fZ=source.fZ;
  }
  return *this;
}