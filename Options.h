#ifndef Options_h
#define Options_h

#include "BinModel.h"
#include "BinDepLattice.h"

class Option
{
   private:
      int N; //steps to expiry

   public:
      void SetN(int N_){N=N_;}
      int GetN(){return N;}
      void StockPriceingTree(BinLatticeDep <double>& Price, BinModel Model); 
      virtual void PayoffTree(BinLatticeDep <double>& Price)=0;
      virtual double Payoff(vector <double>& Path)=0;
       void PriceByCRR(BinLatticeDep <double>& Price,BinModel Model);
       void PathSim(vector <double>& Path, BinModel Model);
       void StockPathSim(vector <double>& Path,BinModel Model);
       void PriceByMC(BinModel Model, double M);
};


class ArthAsianCall : public Option  
{
   private:
   int K;
   public:

   void GetInput();
   void PayoffTree(BinLatticeDep <double>& Price);
   double Payoff(vector <double>& Path);
   void SetK(double K_){K = K_;}
  double GetK(){return K;}
  
};

class LookBack: public Option
{
    private: 
        double K;
    public:
        void GetInput();
        void SetK(double K_){K = K_;}
        double GetK(){return K;}   
        void PayoffTree(BinLatticeDep <double>& Price); 
        double Payoff(vector <double>& Path);
};





#endif
