#include "Vertex.h"
#include <TRandom.h>
ClassImp(Vertex)
Vertex::Vertex()
{
  fMultiplicity = 0;
  for(int i=0;i<3;i++)
    fP0[i]=0;
}
Vertex::Vertex(double x, double y,double z,int mult)
{
  fMultiplicity =mult;
  fP0[0]=x;
  fP0[1]=y;
  fP0[2]=z;
}
//constructor for gaussian primary vertex position and multiplicity from HistoDistribution
Vertex::Vertex(double Sx,double Sy,double Sz,TH1D* HistoDistribution):TObject()
{  
  fMultiplicity = (int)(HistoDistribution->GetRandom());
  fP0[0]=gRandom->Gaus(0,Sx);
  fP0[1]=gRandom->Gaus(0,Sy);
  fP0[2]=gRandom->Gaus(0,Sz);
}

Vertex::~Vertex()
{ 
}
//copy constructor
Vertex& Vertex::operator=(const Vertex& source)
{
  if(this!=&source)
  {
    TObject::operator=(source);
    fMultiplicity=source.fMultiplicity;
    for(int i=0;i<3;i++)
      fP0[i]=source.fP0[i];
  }
  return *this;
}