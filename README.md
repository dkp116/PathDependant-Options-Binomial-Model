# Pricing Path-Dependent Lookback and Arithmetic Asian Options Using CRR and Monte Carlo

## Table of Contents

- [Installation](#installation)
- [Arithmetic Asian Option Pricing](#arithmetic-asian-option-pricing)
- [Lookback Option Pricing](#lookback-option-pricing)
- [License](#license)

## Installation

To use this project, ensure you have a C++ compiler installed on your machine. The project relies on standard libraries and does not require additional dependencies. Follow these steps to set up the project:

1. **Clone the repository:**
   ```bash
   git clone https://github.com/dkp116/PathDependant-Options-Binomial-Model.git


2. Navigate into the project directory and compile the source code:
   ```bash
   cd PathDependant-Options-Binomial-Model
   make
   ```
3. Run the executable:
   ```bash
   ./Main01
   ```
## Arithmetic Asian Option Pricing 

The code includes two pricing algorithms for arithmetic Asian options:

1. CRR (Cox-Ross-Rubinstein) Method: A binomial tree model.
2. Monte Carlo Simulation: A statistical approach.

### Example Code
To price an Arithmetic Asian Call option, use the following code:

```cpp
BinModel Model;
if (Model.GetInputData() == 1) return 1;

BinLatticeDep<double> Tester1;

ArthAsianCall Opt;  // Create an instance of the Arithmetic Asian Call option
Opt.GetInput();     // Input stock price, up/down factors, and risk-free rate
Opt.PriceByCRR(Tester1, Model); // Price using CRR method
Opt.PriceByMC(Model, 1000);     // Price using Monte Carlo Simulation
```

## LookBack Option Pricing

To price a Lookback option, simply switch to the LookBack class:

### Example Code
```cpp
BinModel Model;
if (Model.GetInputData() == 1) return 1;

BinLatticeDep<double> Tester1;

LookBack Opt;  // Create an instance of the Lookback option
Opt.GetInput(); // Input stock price, up/down factors, and risk-free rate
Opt.PriceByCRR(Tester1, Model); // Price using CRR method
Opt.PriceByMC(Model, 1000);     // Price using Monte Carlo Simulation
```
## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE.txt) file for more details.


