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

// Creates a tree of stock prices
void Option::StockPriceingTree(BinLatticeDep<double>& Price, BinModel Model) {
    Price.SetN(N);
    Price.SetNode(0, 0, Model.GetS0());

    for (int z = 0; z < N; ++z) {
        double it = pow(2, z) - 1;
        for (int i = 0; i <= it; ++i) {
            Price.SetNode(z + 1, 2 * i + 1, Price.GetNode(z, i) * (1 + Model.GetU()));
            Price.SetNode(z + 1, 2 * i, Price.GetNode(z, i) * (1 + Model.GetD()));
        }
    }
}

// Prices the option using the CRR method
void Option::PriceByCRR(BinLatticeDep<double>& Price, BinModel Model) {
    auto start = std::chrono::steady_clock::now();
    double N = GetN();
    
    StockPriceingTree(Price, Model);
    PayoffTree(Price);

    for (int n = N; n > 0; --n) {
        for (int i = 0; i <= pow(2, n) - 1; i += 2) {
            double z = (Price.GetNode(n, i) * (1 - Model.RiskNeutProb()) + 
                        Price.GetNode(n, i + 1) * Model.RiskNeutProb()) / 
                       (1 + Model.GetR());
            Price.SetNode(n - 1, i / 2, z);
        }
    }
    
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;

    cout << "CRR Price: " << Price.GetNode(0, 0) << endl;
    cout << "Time taken for CRR Price: " << elapsed_seconds.count() << " seconds" << endl;
    Price.Clear();
}

// Prices the option using Monte Carlo simulation
void Option::PriceByMC(BinModel Model, double M) {
    auto start = std::chrono::steady_clock::now();
    double x = 0;
    vector<double> StorePayOff(M);

    for (int n = 0; n < M; ++n) {
        vector<double> Path;
        PathSim(Path, Model);
        StockPathSim(Path, Model);
        double payoff = Payoff(Path);
        StorePayOff[n] = payoff;
        x += payoff;
    }

    double Avg = x / M;
    double riskNeutral = pow(1 / (1 + Model.GetR()), N);
    
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    
    cout << "MC Estimation: " << Avg * riskNeutral << endl;

    double rootM = sqrt(M);
    double unbiasedEst = 0;

    for (int n = 0; n < M; ++n) {
        unbiasedEst += pow(StorePayOff[n] - Avg, 2);
    }
    unbiasedEst = sqrt(unbiasedEst / (M - 1));

    double StandardError = (unbiasedEst / rootM) * riskNeutral;

    cout << "MC Standard Error: " << StandardError << endl;
    cout << "Time taken for MC Price: " << elapsed_seconds.count() << " seconds" << endl;

    StorePayOff.clear();
}

// Simulates a path of 1s and 0s for stock movements
void Option::PathSim(vector<double>& Path, BinModel Model) {
    Path.resize(N);
    Path[0] = Model.GetS0();

    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> dist(0.0, 1.0);

    for (int i = 1; i <= N; ++i) {
        double random = dist(gen);
        Path[i] = (random <= Model.RiskNeutProb()) ? 1 : 0;
    }
}

// Uses the path of 1s and 0s to model stock price movements
void Option::StockPathSim(vector<double>& Path, BinModel Model) {
    double Initial = Model.GetS0();
    double U = Model.GetU();
    double D = Model.GetD();

    for (int i = 1; i <= N; ++i) {
        if (Path[i] == 1) {
            Initial *= (1 + U);
        } else if (Path[i] == 0) {
            Initial *= (1 + D);
        } else {
            Path[0] = Model.GetS0();
        }
        Path[i] = Initial;
    }
}

// Gets input for the Asian call option
void ArthAsianCall::GetInput() {
    int N;
    cout << "Enter Expiry time: "; cin >> N;
    SetN(N);
    cout << "Enter Strike Price: "; cin >> K;
    SetK(K);
}

// Creates a payoff tree for the Asian call option
void ArthAsianCall::PayoffTree(BinLatticeDep<double>& Price) {
    double N = GetN();

    for (int i = 0; i <= pow(2, N) - 1; ++i) {
        double Sum = Price.GetNode(N, i);
        int z = i;
        
        for (int n = N; n > 1; --n) {
            if (z % 2 == 1) {
                Sum += Price.GetNode(n - 1, (z - 1) / 2);
                z = (z - 1) / 2;
            } else {
                Sum += Price.GetNode(n - 1, z / 2);
                z = z / 2;
            }
        }
        
        double Avg = Sum / N;
        Price.SetNode(N, i, max(Avg - K, 0.0));
    }
}

// Computes the payoff for a given path for the Asian call option
 double ArthAsianCall::Payoff(const vector <double>& Path)
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
// Gets input for the LookBack option
void LookBack::GetInput() {
    int N;
    cout << "Enter Expiry time: "; cin >> N;
    SetN(N);
    cout << "Enter Strike Price: "; cin >> K;
    SetK(K);
}

// Creates a payoff tree for the LookBack option
void LookBack::PayoffTree(BinLatticeDep<double>& Price) {
    double N = GetN();

    for (int i = 0; i < pow(2, N); ++i) {
        double x = Price.GetNode(N, i);
        double y = x;
        int z = i;
        
        for (int n = N; n >= 1; --n) {
            if (z % 2 == 1) {
                y = min(y, Price.GetNode(n - 1, (z - 1) / 2));
                z = (z - 1) / 2;
            } else {
                y = min(y, Price.GetNode(n - 1, z / 2));
                z = z / 2;
            }
        }
        
        Price.SetNode(N, i, max(x - y, 0.0));
    }
}

// Computes the payoff for a given path for the LookBack option
double LookBack::Payoff(const vector<double>& Path) {
    double N = GetN();
    double Terminal = Path[N];
    double Min = *min_element(Path.begin(), Path.begin() + N + 1);
    return max(Terminal - Min, 0.0);
}

