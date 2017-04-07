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

	vector<list<node> > m;
	
	int size; // sin tener en cuenta la columna del b

public:

	SparseMatrix(vector<vector<double> > a, int dim);
	//~SparseMatrix(void);

	void show();
	void scalarMult(double a);
	void scalarRowMult(double a, int row);
	void rowSub(int pivotRow, int lowerRow, double k);

	int getSize();
	int getFirstPos(int row);
	int getLastPos(int row);
	double getFirstVal(int row);
	double getLastVal(int row);

	bool isZero(double k);

	void eg(double r[]);
	void cl();
	void backward_substitution(double r[]);
	void forward_substitution(double r[]);

};


#endif
