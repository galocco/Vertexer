#include "BeamPipe.h"
#include "Particle.h"
ClassImp(BeamPipe)
BeamPipe::BeamPipe()
{
  fRadius =0;
  fThickness =0;
  fSigmaScattering = 0;
}
BeamPipe::BeamPipe(double R,double S,double theta)
{
  fRadius = R;
  fThickness = S;
  fSigmaScattering = theta;
}
BeamPipe::~BeamPipe()
{
}
//function to simulate the multiple scattering changing the parametric line of the particle
void BeamPipe::Scattering(Particle* P)
{
  //angles of scattering
  double ThetaDev = gRandom->Gaus(0,fSigmaScattering);
  double PhiDev = 2*TMath::Pi()*gRandom->Rndm();
  //rotation of the line
  double mr[3][3];
  double Pphi=P->GetPhi();
  double Ptheta=P->GetTheta();
  mr[0][0]=-TMath::Sin(Pphi);
  mr[1][0]=TMath::Cos(Pphi);
  mr[2][0]=0.;
  mr[0][1]=-TMath::Cos(Pphi)*TMath::Cos(Ptheta);
  mr[1][1]=-TMath::Cos(Ptheta)*TMath::Sin(Pphi);
  mr[2][1]=TMath::Sin(Ptheta);
  mr[0][2]=P->GetDirection(0);
  mr[1][2]=P->GetDirection(1);
  mr[2][2]=P->GetDirection(2);
  double cdp[3];
  cdp[0]=TMath::Sin(ThetaDev)*TMath::Cos(PhiDev);
  cdp[1]=TMath::Sin(ThetaDev)*TMath::Sin(PhiDev);
  cdp[2]=TMath::Cos(ThetaDev);
  //solution of the system cylinder-line
  double t=GetIntersectionParameter(P);
  //change of the parametric line
  for(int i=0;i<3;i++)
  {
    double NewDir=0;
    P->SetP0(i,t*P->GetDirection(i)+P->GetP0(i));
    for(int j=0;j<3;j++)
    {
      NewDir+=mr[i][j]*cdp[j];
    }
    P->SetDirection(i,NewDir);
  }
  P->SetTheta(TMath::ACos(P->GetDirection(2)));
  P->SetPhi(TMath::ACos(P->GetDirection(0)/TMath::Sin(P->GetTheta())));

}
//the function return the value of the t parameter in the intersection between particle and beampipe
double BeamPipe::GetIntersectionParameter(Particle* P)
{
  double x[3];
  double c[3];
  for(int i=0;i<=2;i++)
  {
    x[i]=P->GetP0(i);
    c[i]=P->GetDirection(i);
  }
  //solution of the system
  double delta = (x[0]*c[0]+x[1]*c[1])*(x[0]*c[0]+x[1]*c[1])-(c[0]*c[0]+c[1]*c[1])*(x[0]*x[0]+x[1]*x[1]-(fRadius+fThickness)*(fRadius+fThickness));
  //return t
  return (-(x[0]*c[0]+x[1]*c[1])+TMath::Sqrt(delta))/(c[0]*c[0]+c[1]*c[1]); 
}
//copy constructor
BeamPipe& BeamPipe::operator=(const BeamPipe& source)
{
  if(this!=&source)
  {
    TObject::operator=(source);
    fRadius=source.fRadius;
    fThickness=source.fThickness;
    fSigmaScattering=source.fSigmaScattering;
  }
  return *this;
}