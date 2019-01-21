//macro to simplify the use
void LoadAll()
{
  gROOT->ProcessLine(".L Particle.cxx+");
  gROOT->ProcessLine(".L BeamPipe.cxx+");
  gROOT->ProcessLine(".L Layer.cxx+");
  gROOT->ProcessLine(".L HitPoint.cxx+");
  gROOT->ProcessLine(".L Vertex.cxx+");
  gROOT->ProcessLine(".L MonteCarlo.cxx+");
  gROOT->ProcessLine(".L DeltaPhiAnalysis.C+");
  gROOT->ProcessLine(".L MCGenerator.C+");
  gROOT->ProcessLine(".L MCTransport.C+");
  gROOT->ProcessLine(".L MCAnalysis.C+");
  gROOT->ProcessLine(".L DeltaZAnalysis.C+");
  gROOT->ProcessLine(".L MCHistos.C+");
}
//NEvents = number of primary verteces
//eta= pseudorapidity in [-eta,eta]
//Seed di TRandom
//Verb=true to print more information about the process
//Real=true to use the real multiplicity distribution
void RunAll(const int NEvents=100000,const float eta=2,const int Seed=482,const bool Verb=true,const bool Real=false)
{
  gROOT->ProcessLine(Form("MonteCarlo Analysis(%i,%i,true,false,%.2f);",NEvents,Seed,eta));
  gROOT->ProcessLine("Analysis.MCGenerator()");
  gROOT->ProcessLine("Analysis.MCTransport(true,true,true,50,50)");
  gROOT->ProcessLine("Analysis.DeltaPhiAnalysis(0.0001,0.01,0.0001,1000)");
  gROOT->ProcessLine("Analysis.DeltaZAnalysis(10000)");
  gROOT->ProcessLine("Analysis.MCAnalysis(50)");
  gROOT->ProcessLine("Analysis.MCHistos(true,50)");
}
