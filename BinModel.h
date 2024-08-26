#ifndef BinModel_h
#define BinModel_h

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
   
      //inputting, displaying and checking model data
      int GetInputData();

      double GetR();
      double GetU();
      double GetD();
      double GetS0();
};

#endif
