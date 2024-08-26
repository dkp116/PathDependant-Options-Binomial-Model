main: Main.o BinModel.o Options.o 
	g++  Main.o BinModel.o Options.o -o Main01


Main.o : Main.cpp 
	g++ -c Main.cpp

BinomialModel.o : BinModel.cpp BinModel.h 
	g++ -c BinoModel.cpp

Option.o : Options.cpp Options.h BinDepLattice.h 
	g++ -c  Option.cpp

clean:
	rm *.o Main01