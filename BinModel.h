#ifndef BinModel_h
#define BinModel_h
class BinModel {
private:
    double S0;  // Initial stock price
    double U;   // Up factor
    double D;   // Down factor
    double R;   // Risk-free interest rate

public:
    // Computes the risk-neutral probability
    double RiskNeutProb();

    // Computes the stock price at node (n, i)
    double S(int n, int i);

    // Placeholder for a potential method to compute stock price at node (n, i, d)
    double S(int n, int i, int d);

    // Inputs, displays, and checks model data
    int GetInputData();

    // Accessor methods
    double GetR();
    double GetU();
    double GetD();
    double GetS0();
};
#endif
