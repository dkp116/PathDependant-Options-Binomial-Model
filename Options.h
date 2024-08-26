#ifndef Options_h
#define Options_h

#include "BinModel.h"
#include "BinDepLattice.h"

class Option {
private:
    int N; // Number of steps to expiry

public:
    // Set the number of steps to expiry
    void SetN(int N_) { N = N_; }

    // Get the number of steps to expiry
    int GetN() const { return N; }

    // Construct the stock price lattice
    void StockPriceingTree(BinLatticeDep<double>& Price, BinModel Model);

    // Pure virtual functions to be implemented by derived classes
    virtual void PayoffTree(BinLatticeDep<double>& Price) = 0;
    virtual double Payoff(const vector<double>& Path) = 0;

    // Price the option using the CRR method
    void PriceByCRR(BinLatticeDep<double>& Price, BinModel Model);

    // Simulate the path of the option
    void PathSim(vector<double>& Path, BinModel Model);

    // Simulate the stock path
    void StockPathSim(vector<double>& Path, BinModel Model);

    // Price the option using Monte Carlo simulation
    void PriceByMC(BinModel Model, double M);
};

class ArthAsianCall : public Option {
private:
    double K; // Strike price

public:
    // Get input for the option
    void GetInput();

    // Calculate the payoff tree for the option
    void PayoffTree(BinLatticeDep<double>& Price);

    // Calculate the payoff for a given path
    double Payoff(const vector<double>& Path);

    // Set the strike price
    void SetK(double K_) { K = K_; }

    // Get the strike price
    double GetK() const { return K; }
};

class LookBack : public Option {
private:
    double K; // Strike price

public:
    // Get input for the option
    void GetInput();

    // Set the strike price
    void SetK(double K_) { K = K_; }

    // Get the strike price
    double GetK() const { return K; }

    // Calculate the payoff tree for the option
    void PayoffTree(BinLatticeDep<double>& Price);

    // Calculate the payoff for a given path
    double Payoff(const vector<double>& Path);
};

#endif