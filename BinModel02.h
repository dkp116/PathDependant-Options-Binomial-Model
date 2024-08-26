#ifndef BinModel02_h
#define BinModel02_h

class BinModel
{
   private:
      double S0;
      double U;
      double D;
      double R;

   public:
      //computing risk-neutral probability
      double RiskNeutProb();

      //computing the stock price at node n,i
      double S(int n, int i); 

      double S(int n, int i, int d);
     //But we need the exact sequence, hmm 
     // so s1u s1d s2uu s2ud s2du s2dd be able to fill in each node hmmm

      //inputting, displaying and checking model data
      int GetInputData();

      double GetR();
      double GetU();
      double GetD();
      double GetS0();
};

#endif
