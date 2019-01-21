//object for the beampipe
#ifndef BeamPipe_H
#define BeamPipe_H

#include <TMath.h>
#include <TRandom.h>
#include "Particle.h"
#include <TObject.h>
class BeamPipe : public TObject{
  public:
    BeamPipe();
    BeamPipe(double R,double S,double theta);
    BeamPipe& operator=(const BeamPipe& source);
    virtual ~BeamPipe();
    void Scattering(Particle* P);

    double GetRadius(){return fRadius;};
    void SetRadius(double R){fRadius=R;};
    double GetThickness(){return fThickness;};
    void SetThickness(double S){fThickness=S;};
    double GetSigmaScattering(){return fSigmaScattering;};
    void SetSigmaScattering(double theta){fSigmaScattering=theta;};
  protected:
  //return the value of the parameter t in the intersection between particle and beampipe
    double GetIntersectionParameter(Particle* P);
  // beampipe radius
    double fRadius;
  // beampipe thickness
    double fThickness;
  // standard deviation of the scattering angle
    double fSigmaScattering;
  ClassDef(BeamPipe,1);
};


#endif