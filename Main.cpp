#include "BinDepLattice.h"
#include "BinModel.h"
#include "Options.h"
#include <iostream>
#include <cstdlib>
using namespace std;

int main()
{
  BinModel Model;
  if (Model.GetInputData()==1) return 1;

  BinLatticeDep<double> Tester1;
 LookBack Opt; //Pick which option you want to price, LookBack OR ArthAsianCall
  Opt.GetInput();
  double Sim = 1000; //No of simulations 
  Opt.PriceByMC(Model,Sim);
  Opt.PriceByCRR(Tester1,Model);
   

}

