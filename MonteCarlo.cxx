#include "MonteCarlo.h"
ClassImp(MonteCarlo)
MonteCarlo::MonteCarlo():TObject()
{
  fSeed=0;
  fPrimaryVertices=0;
  fVerbose=false;
  fRealDistribution=false;
  fDeltaPhi=0;
  fZ0BinWidth=0;
  fEta=0;
}
MonteCarlo::MonteCarlo(int NVertices,int Seed,bool Verbose,bool Real,double Eta):TObject()
{
  fSeed=Seed;
  fPrimaryVertices=NVertices;
  fVerbose=Verbose;
  fRealDistribution=Real;
  fDeltaPhi=0;
  fZ0BinWidth=0;
  fEta=Eta;
}
MonteCarlo::MonteCarlo(int NVertices,int Seed,bool Verbose,bool Real,double Eta,double Phi,double Width):TObject()
{
  fSeed=Seed;
  fPrimaryVertices=NVertices;
  fVerbose=Verbose;
  fRealDistribution=Real;
  fDeltaPhi=Phi;
  fZ0BinWidth=Width;
  fEta=Eta;
}

MonteCarlo::~MonteCarlo()
{
}
//copy constructor 
MonteCarlo& MonteCarlo::operator=(const MonteCarlo& source)
{
  if(this!=&source)
  {
    TObject::operator=(source);
    fPrimaryVertices=source.fPrimaryVertices;
    fVerbose=source.fVerbose;
    fSeed=source.fSeed;
    fRealDistribution=source.fRealDistribution;
    fDeltaPhi=source.fDeltaPhi;
    fZ0BinWidth=source.fZ0BinWidth;
    fEta=source.fEta;
  }
  return *this;
}