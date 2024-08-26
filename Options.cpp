#include "Options.h"
#include "BinModel.h"
#include "BinDepLattice.h"
#include <iostream>
#include <cmath>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <random>
#include <chrono>
using namespace std;


void Option::StockPriceingTree(BinLatticeDep <double>& Price, BinModel Model)  //Creates tree of stock prices
{
    Price.SetN(N);
    Price.SetNode(0,0,Model.GetS0());
    for (int z = 0; z <  N;z++) 
    { double it = pow(2,z) - 1;
    for (int i =0; i <= it ;i++) 
    {  
        Price.SetNode(z+1,2*i+1,Price.GetNode(z,i)*(1+Model.GetU()));
        Price.SetNode(z+1,2*i,Price.GetNode(z,i)*(1+Model.GetD()));
    }
    }

    
};

 void Option::PriceByCRR(BinLatticeDep <double>& Price,BinModel Model) //Uses payofftree to price by CRR
  {   auto start = std::chrono::steady_clock::now();
    double N = GetN();
   StockPriceingTree(Price,Model);
  
    PayoffTree(Price);
    

   for(int n = N; n>0 ; n--)
   {
      for(int i =0 ; i<=pow(2,n)-1; i = i +2)
      {
         double z = (Price.GetNode(n,i)*(1-Model.RiskNeutProb())+Price.GetNode(n,i+1)*Model.RiskNeutProb())/(1+Model.GetR());
         Price.SetNode(n-1,i/2,z);
      }
   }
   auto end = std::chrono::steady_clock::now();
       std::chrono::duration<double> elapsed_seconds = end - start;

   cout << "CRR Price "  << Price.GetNode(0,0) << endl;
   cout << "The time taken for CRR Price " << elapsed_seconds.count() << " seconds" << endl;
   Price.Clear();
  };


   void Option::PriceByMC(BinModel Model, double M)
  {  auto start = std::chrono::steady_clock::now();
   double x = 0;
  vector< double> StorePayOff (M);
   for (int n = 0 ; n < M ; n++) 
  {
   
   vector <double> Path;
   PathSim(Path,Model);
   StockPathSim(Path,Model);
   StorePayOff[n] = Payoff(Path);
    x += Payoff(Path);
  }

  double Avg = x /M;
  double riskNeutral = pow(1/(1+Model.GetR()),N);
  auto end = std::chrono::steady_clock::now();
       std::chrono::duration<double> elapsed_seconds = end - start;
  cout << "MC Estimation is " <<  Avg * riskNeutral << endl;

   double rootM = sqrt(M);

  
double unbiasedEst = 0; // add unbaised estimator calc difference between path and average, create a vecotor and store all values inside
 for (int n = 0 ; n < M ; n++)
 {
   unbiasedEst +=pow(StorePayOff[n] - Avg , 2);

 }

 unbiasedEst = sqrt(unbiasedEst/(M-1));

double StandardError = (unbiasedEst / rootM) * riskNeutral;


  cout << "MC Standard Error is " << StandardError << endl;

   cout << "The time taken for MC Price " << elapsed_seconds.count() << " seconds" << endl; 
   StorePayOff.clear();
     
  };

  void Option::PathSim(vector <double>& Path,BinModel Model) //Return series of 1 and 0 for a path with stock at the start 
  {
   Path.resize(N);

    Path[0] = Model.GetS0();

    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> dist(0.0, 1.0);

    for (int i = 1; i <= N; i++)
    {
        double random = dist(gen);
        if (random <=  Model.RiskNeutProb())
            Path[i] = 1;
        else
            Path[i] = 0;
    }
  };

  void Option::StockPathSim(vector <double>& Path,BinModel Model) //Use the vector of 1 and 0 to model stock price up and down
  {
   double Initial = Model.GetS0();
   double U = Model.GetU();
   double D = Model.GetD();
   for(int i = 1; i <= N ; i++ )
   {
      if(Path[i]== 1 ){Initial = Initial * (1+ U); Path[i] = Initial;}
      else if(Path[i] == 0 ){Initial = Initial * ( 1+D); Path[i]= Initial;}
      else{Path[0] =Model.GetS0();}
      
   }

  
  };



void ArthAsianCall::GetInput()
{
   int N;
   cout << "Enter Expiry time " ; cin >> N;
   SetN(N);
   cout << "Enter Strike Price "; cin >> K;
   SetK(K);
};




void ArthAsianCall::PayoffTree(BinLatticeDep <double>& Price) //Creates a payoff tree for a given stock tree
{
     double N = GetN();
     
      
      for (int i=0 ; i <=pow(2,N) - 1;i++){ 
         double Sum = Price.GetNode(N,i); 
         
         int z = i;
         for (int n = N; n > 1 ; n--)
         {  
             
            if(z%2 == 1){ Sum += Price.GetNode(n-1,(z-1)/2); z =(z-1)/2;} 
            else{ Sum += Price.GetNode(n-1,z/2); z = z/2;}  
            
         }
      double Avg = Sum/N;
      
      if(Avg - K > 0 ){Price.SetNode(N,i,Avg-K);}
      else {Price.SetNode(N,i,0.0);}
      

   }
}
 

 double ArthAsianCall::Payoff(vector <double>& Path)
{
   double N = GetN();
   double pay = 0;
  for (int  i = 1;  i <= N; i++) 
  {
   pay += Path[i];
  }
  
  if((pay/N)-K>0){ return (pay/N)-K;}
  else{ return 0;}

 
}




void  LookBack::GetInput()
{
   int N;
   cout << "Enter Expiry time " ; cin >> N;
   SetN(N);
   cout << "Enter Strike Price "; cin >> K;
   SetK(K);
};




  
void   LookBack::PayoffTree(BinLatticeDep <double>& Price) 
{
    double N = GetN();
   
   for (int i=0 ; i < pow(2,N);i++) 
      { double x = Price.GetNode(N,i); 
      double& y = x; 
         y = Price.GetNode(N,i);
         int z = i;
         for (int n = N; n >= 1 ; n--)
         {
            
            if(z%2 == 1){  y = min(y,Price.GetNode(n-1,(z-1)/2)); z = (z-1)/2;} 
            else{ y = min(y,Price.GetNode(n-1,z/2)); z = z/2;}
            
         }
         double Pay = Price.GetNode(N,i) - y;
         {Price.SetNode(N,i,Pay);}
                 
      }
};


 double LookBack::Payoff(vector <double>& Path)
{
   
   double N = GetN();
   double Terminal = Path[N];
   double Min = Terminal;

  for (int i =0 ; i <= N; i++)   
  {
   Min = min(Min, Path[i]);
  }
  
  return Terminal - Min; 
}




