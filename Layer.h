//object for the deterctor
#ifndef LAYER_H
#define LAYER_H

#include "Particle.h"
#include "BeamPipe.h"
class Layer: public BeamPipe{
  public:
    Layer();
    Layer(double L,double R,double T,double Sz,double Spr,double BG,double Eff,double S);
    Layer& operator=(const Layer& source);

    virtual ~Layer();
    double GetLenght(){return fLenght;};
    void SetLenght(double L){fLenght=L;};
    double GetSmearingZ(){return fSmearingZ;};
    void SetSmearingZ(double S){fSmearingZ=S;};
    double GetSmearingPhiR(){return fSmearingPhiR;};
    void SetSmearingPhiR(double S){fSmearingPhiR=S;};
    double GetBackground(){return fBackground;};
    void SetBackground(double B){fBackground=B;};
    double GetEfficiency(){return fEfficiency;};
    void SetEfficiency(double Eff){fEfficiency=Eff;};
  
  private:
  // detector lenght
    double fLenght;
  // standard deviation for the smearing values
    double fSmearingZ;
    double fSmearingPhiR;
  //number of false signals for each primary vertex
    double fBackground;
  // effinciency of revelation
    double fEfficiency;

  ClassDef(Layer,1);
};


#endif
