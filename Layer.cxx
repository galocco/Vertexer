#include "Layer.h"
ClassImp(Layer)
Layer::Layer()
{
  fLenght = 0;
  fRadius = 0;
  fThickness = 0;
  fSmearingPhiR = 0;
  fSmearingZ = 0;
  fBackground = 0;
  fEfficiency = 0;
  fSigmaScattering = 0;
}
Layer::Layer(double L,double R,double T,double Sz,double Spr,double BG,double Eff,double S)
{
  fLenght = L;
  fRadius = R;
  fThickness = T;
  fSmearingPhiR = Spr;
  fSmearingZ = Sz;
  fBackground = BG;
  fEfficiency = Eff;
  fSigmaScattering = S;
}
Layer::~Layer()
{
  
}
//copy constructor
Layer& Layer::operator=(const Layer& source)
{
  if(this!=&source)
  {
    TObject::operator=(source);
    fRadius=source.fRadius;
    fThickness=source.fThickness;
    fBackground=source.fBackground;
    fLenght=source.fLenght;
    fSmearingZ=source.fSmearingZ;
    fSmearingPhiR=source.fSmearingPhiR;
    fEfficiency=source.fEfficiency;
    fSigmaScattering=source.fSigmaScattering;
  }
  return *this;
}