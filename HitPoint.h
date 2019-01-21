//object for the signal
#ifndef HITPOINT_H
#define HITPOINT_H
#include <TMath.h>
#include <TRandom.h>
#include <TObject.h>
#include "Particle.h"
#include "Layer.h"
class HitPoint : public TObject {
  public:
    HitPoint();
    HitPoint(Layer L,Particle *P);
    HitPoint(Layer L);
    HitPoint(double Z,double Phi,double R);
    HitPoint& operator=(const HitPoint& source);
    virtual ~HitPoint();
    //smearing simulation 
    void Smearing(Layer L);

    double GetZ(){ return fZ;};
    void SetZ(double z){ fZ = z;};
    double GetR(){return fR;};
    void SetR(double t){fR = t;};
    double GetPhi(){return fPhi;};
    void SetPhi(double f){fPhi = f;};
  private:
  //cylindrical coordinates of the signal
    double fZ;
    double fPhi;
    double fR;
  ClassDef(HitPoint,1);
};


#endif
