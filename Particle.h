/*
object for the particle
*/
#ifndef PARTICLE_H
#define PARTICLE_H
#include <TMath.h>
#include <TH1D.h>
#include <TRandom.h>
#include <TObject.h>
#include "Vertex.h"
class Particle : public TObject {
  public:
    Particle();
    Particle(double p0[],TH1D* HistoDistribution,double EtaMin,double EtaMax);//inutile
    Particle(Vertex V,TH1D* HistoDistribution,double EtaMin,double EtaMax);
    Particle(double p0[],double theta,double phi);
    Particle& operator=(const Particle& source);
    virtual ~Particle();
    double GetP0(int i){ return fX[i];};
    void SetP0(int i,double x){ fX[i] = x;};
    double GetDirection(int i){ return fC[i];};
    void SetDirection(int i,double c){ fC[i] = c;};
    double GetTheta(){return fTheta;};
    void SetTheta(double t){fTheta=t;};
    double GetPhi(){return fPhi;};
    void SetPhi(double p){fPhi=p;};
    
  private:
  //particle motion caracterized by a parametric line xi=fX[i]+fC[i]*t
    double fX[3];
    double fC[3];
  //paramerters for  fC[]
    double fTheta;
    double fPhi;
    ClassDef(Particle,1);
};


#endif