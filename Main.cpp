#include "BinDepLattice.h"
#include "BinModel.h"
#include "Options.h"
#include <iostream>
#include <cstdlib>

using namespace std;

int main() {
    BinModel Model;
    if (Model.GetInputData() == 1) return 1;

    BinLatticeDep<double> Tester1;

   ArthAsianCall Opt;  // Choose the option to price: LookBack or ArthAsianCall
    Opt.GetInput();
    Opt.PriceByCRR(Tester1,Model);
    Opt.PriceByMC(Model,1000);
}
