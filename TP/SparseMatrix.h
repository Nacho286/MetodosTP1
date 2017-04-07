#include <iostream>
#include <list>
#include <vector>
#include <cstdlib>
#include <math.h>
using namespace std;

#ifndef MATRIX_H
#define MATRIX_H

class SparseMatrix{


private:

	struct node{
		double value; 
		int pos;
	};

	std::vector<std::list<node> > m;
	
	int size; // sin tener en cuenta la columna del b

public:


	SparseMatrix(std::vector<std::vector<double> > a, int dim);
	int getSize();
	void show();
	void scalarMult(double a);
	void ScalarRowMult(double a, int row);
	int getFirstPos(int row);
	double getFirstVal(int row);
	int getLastPos(int row);
	double getLastVal(int row);
	void rowSub(int pivotRow, int lowerRow, double k);
	bool isZero(double k);
	void eg(double r[]);
	void backward_substitution(double r[]);

};


#endif