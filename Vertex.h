/*
object for the primary vertex
*/
#ifndef Vertex_H
#define Vertex_H
#include <TObject.h>
#include <TH1.h>
#include <TMath.h>
class Vertex : public TObject{
  public:
    Vertex();
    Vertex(double Sx,double Sy,double Sz,TH1D* HistoDistribution);
    Vertex(double x,double y,double z,int mult);
    Vertex& operator=(const Vertex& source);
    virtual ~Vertex();
    int GetMultiplicity(){return fMultiplicity;};
    void SetMultiplicity(int N){fMultiplicity=N;};
    double GetP0(int i){return fP0[i];}
    void SetP0(int i, double x){fP0[i]=x;};
  private:
    int fMultiplicity;
    //primary vertex position (x,y,z) 
    double fP0[3];
  ClassDef(Vertex,1);
};

#endif