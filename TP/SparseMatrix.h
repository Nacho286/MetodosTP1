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
	
	int rowSize; 
	int colSize;
	double dotProduct(int i, int j, double diagonal);

public:

	SparseMatrix(vector<vector<double> > a, int rowSize, int colSize);
	SparseMatrix(int rowDim, int colDim);
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
	void addtoRow(int row, node& n);

	bool isZero(double k);

	void eg(double r[]);
	SparseMatrix cl();
	void solve_cl(double r[]);
	void backward_substitution(double r[]);
	void backward_substitution_cl(double r[]);
	void forward_substitution_cl(double r[]);

};


#endif
