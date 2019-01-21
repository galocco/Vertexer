//object for the analysis
#ifndef MONTECARLO_H
#define MONTECARLO_H
#include <TObject.h>
#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include "HitPoint.h"
//struct for the primary vertex data saving
typedef struct{
  double Multiplicity;
  double Z0;
} VertexData;

class MonteCarlo: public TObject{
  public:
    MonteCarlo();
    MonteCarlo(int NVertices,int Seed,bool Verbose,bool Real,double Eta);
    MonteCarlo(int NVertices,int Seed,bool Verbose,bool Real,double Eta,double Phi,double Width);
    MonteCarlo& operator=(const MonteCarlo& source);
    virtual ~MonteCarlo();
    //metod for the events generation
    void MCGenerator();
    //metod for the transport simulation
    void MCTransport(const bool Scatt,const bool Smea,const bool Noise,const int N1,const int N2);
    //metod for the primary vertex z analysis
    void MCAnalysis(const int dNoise);
    //metod to set fDeltaPhi and fZ0BinWidth
    void DeltaPhiAnalysis(const double PhiIn,const double PhiFin,const double dPhi,const int NumberOfVertices);
    void DeltaZAnalysis(const int);
    //metod for the histograms filling
    void MCHistos(const bool Projection ,const int dNoise);

    void SetPrimaryVertices(int N){fPrimaryVertices= N;};
    int GetPrimaryVertices(){return fPrimaryVertices;};
    void SetSeed(int S){fSeed=S;};
    int GetSeed(){return fSeed;};
    void SetVerbose(bool V){fVerbose=V;};
    bool GetVerbose(){return fVerbose;};
    void SetRealDistribution(bool R){fRealDistribution=R;};
    bool GetRealDistribution(){return fRealDistribution;};
    void SetDeltaPhi(double Phi){fDeltaPhi=Phi;};
    double GetDeltaPhi(){return fDeltaPhi;};
    void SetZ0BinWidth(double W){fZ0BinWidth=W;};
    double GetZ0BinWidth(){return fZ0BinWidth;};
    void SetPseudorapidity(double E){fEta=E;};
    double GetPseudorapidity(){return fEta;};

  private:
    //metod to calculate the z given two signal
    double ReconstructionZ(HitPoint* P1,HitPoint* P2);
    //metod to produce the TH1D for the resolution
    void RisolutionTH3(TH3D* Histo3,TH2D* Histo2,TH1* Histo1,int dNoise,char Name[]);
    //metod to produce the TH1D for the efficiency
    void EfficiencyTH2(TH2D* Histo2Par,TH2D* Histo2Tot,TH1D* Histo1,int dNoise,char Name[]);
    //number of primary vertices
    int fPrimaryVertices;
    //TRandom seed
    int fSeed;
    //if setted to true more information will be printed
    bool fVerbose;
    //if true moltiplicity will be extracted from the real distribution
    bool fRealDistribution;
    //maximal distance in phi to pair two signals
    double fDeltaPhi;
    //binwidth for the histogram filled by the zs measured 
    double fZ0BinWidth;
    //to define the psedurapidity range[-fEta,+fEta]
    double fEta;

};

double ReconstructionZ(HitPoint* P1,HitPoint* P2)
{
  double z1=P1->GetZ();
  double z2=P2->GetZ();
  double r1=P1->GetR();
  double r2=P2->GetR();
  return (z1*r2-z2*r1)/(r2-r1);
}


#endif
