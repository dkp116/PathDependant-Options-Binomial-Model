#ifndef BinDepLattice_h
#define BinDepLattice_h

#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>

using namespace std;

template<typename Type>
class BinLatticeDep {
private:
    int N;
    vector<vector<Type> > Lattice;

public:
    void SetN(int N_) {
        N = N_;
        Lattice.resize(N + 1);
        for (int n = 0; n <= N; n++) {
            Lattice[n].resize(static_cast<int>(pow(2, n)));
        }
    }

    void SetNode(int n, int i, Type x) {
        Lattice[n][i] = x;
    }

    Type GetNode(int n, int i) {
        return Lattice[n][i];
    }

    void Display() {
        cout << setiosflags(ios::fixed) << setprecision(3);
        for (int n = 0; n <= N; n++) {
            for (int i = 0; i <= pow(2, n) - 1; i++) {
                cout << setw(7) << GetNode(n, i);
            }
            cout << endl;
        }
        cout << endl;
    }

    void Clear() {
        // Clear the Lattice
        // for (int n = 0; n <= N; n++) {
        //     for (int i = 0; i <= pow(2, n) - 1; i++) {
        //         Lattice[n].clear(); // or reset individual nodes as needed
        //     }
        // }
        Lattice.clear();
    }
};

#endif