#include <iostream>
#include <list>
#include <vector>
#include <cstdlib>
#include <math.h>
#include "SparseMatrix.h"
using namespace std;

	
	
	SparseMatrix::SparseMatrix(vector<vector<double> > a, int rowDim, int colDim){
	   
	
	    for(int i = 0; i < rowDim; i++){
	        list<node> row;
	        for(int j = 0; j < colDim; j++){
	            if(!isZero(a[i][j])){
	            	// node *n = new node;
	            	// n->value = a[i][j];
	            	// n->pos = j;
	             	// row.push_back(*n);
	             	node n = node();
	             	n.value = a[i][j];
	             	n.pos = j;
	             	row.push_back(n);
	            }    
	        }
	        m.push_back(row);
	    }
	    rowSize = rowDim;
	    colSize	= colDim;
	}

	SparseMatrix::SparseMatrix(int rowDim, int colDim){
		for(int i = 0; i< rowDim;i++)
		m.push_back(list<node>());
		rowSize = rowDim;
	    colSize	= colDim;

	}

	// SparseMatrix::~SparseMatrix(){
	// 	for(int i = 0; i < size; i++){
	// 		list<node>::iterator itRow = m[i].begin();
	// 		while(itRow != m[i].end()){
	// 			delete &(itRow);
	// 			itRow++;
	// 		}
	// 		m[i].clear();
	// 	}
	// }



	//Imprime la matriz
	void SparseMatrix::show(){

		for(int i = 0; i < rowSize; i++){
	        list<node>::iterator row = m[i].begin();
	        for(int j = 0; j < colSize; j++){
	            if(row->pos == j){
	            	cout << ' ' << row->value;
	            	++row;
	            } else
	            	cout << ' ' << 0; 
	        }
	      	cout << endl;
	    }
	    cout << endl;
	}

	//Multiplica la matriz por un escalar (k*A, k un real) 
	void SparseMatrix::scalarMult(double a){
		vector< list<node> >::iterator row;
		list<node>::iterator col;
		for (row = m.begin(); row != m.end(); row++) {
		    for (col = row->begin(); col != row->end(); col++) 
		           col->value = col->value *a;
		}
	    
	}

	//Multiplica la fila row de m por el escalar a 
	void SparseMatrix::scalarRowMult(double a, int row){
		list<node>::iterator itRow;
		for (itRow = m[row].begin(); itRow != m[row].end(); itRow++)
			itRow->value = itRow->value * a ;		
	}

	int SparseMatrix::getFirstPos(int row){
		return m[row].front().pos;
	}

	int SparseMatrix::getLastPos(int row){
		return m[row].back().pos;
	}

	double SparseMatrix::getFirstVal(int row){
		return m[row].front().value;
	}

	double SparseMatrix::getLastVal(int row){
		return m[row].back().value;
	}

	//Resta fila lowerRow - k*pivotRow
	void SparseMatrix::rowSub(int pivotRow, int lowerRow, double k){

		//se copia la fila
		list<node> lpivotRow = m[pivotRow];

		//Inicializacion de iteradores
		list<node>::iterator itPivotRow = lpivotRow.begin();
		list<node>::iterator itLowerRow = m[lowerRow].begin();

		while(itPivotRow != lpivotRow.end() && itLowerRow != m[lowerRow].end()){
			if (itLowerRow->pos == itPivotRow->pos){
				itLowerRow->value -= (k * itPivotRow->value);

				if(isZero(itLowerRow->value)){
					//node* n = &(*itLowerRow);
					itLowerRow = m[lowerRow].erase(itLowerRow);
					//delete n;
				} else
					++itLowerRow;
            	++itPivotRow;
			} else if (itLowerRow->pos > itPivotRow->pos){
        		// node *n = new node;
	         	// n->value = -k * (itPivotRow->value);
	         	// n->pos = itPivotRow->pos;
        		// m[lowerRow].insert(itLowerRow,*n);
        		node n = node();
	            n.value = -k * (itPivotRow->value);
	            n.pos = itPivotRow->pos;
        		m[lowerRow].insert(itLowerRow, n);

        		++itPivotRow;
        	} else
        		++itLowerRow;
		}
    	
    	//Si termine de recorrer la fila a restar (lowerRow) y quedaron elementos en pivotRow, los agrego.
   		while(itPivotRow != lpivotRow.end()){
   			// node * n = new node;
	     	// n->value = -k * (itPivotRow->value);
	     	// n->pos = itPivotRow->pos;
      		// m[lowerRow].insert(itLowerRow,*n);
   			node n = node();
	        n.value = -k * (itPivotRow->value);
	        n.pos = itPivotRow->pos;
        	m[lowerRow].insert(itLowerRow, n);

        	++itPivotRow;
     	}
 
	}

	void SparseMatrix::backward_substitution(double r[]){
		for (int i = rowSize - 1; i >= 0; i--){
			if(getLastPos(i)!= rowSize)
				r[i] = 0;
			else
				r[i] = getLastVal(i);	
			
			list<node>::iterator it = m[i].end();
			--it;
			while(it->pos != i){
				r[i] -= it->value * r[it->pos];
				--it;
			}
			
			r[i] = r[i] / it->value;
		}
	}

	void SparseMatrix::backward_substitution_cl(double r[]){

		for (int i = rowSize - 1; i >= 0; i--){		
			list<node>::iterator it = m[i].end();
			--it;
			while(it->pos != i){
				r[i] -= it->value * r[it->pos];
				--it;
			}
			
			r[i] = r[i] / it->value;
		}
	}
	
	void SparseMatrix::forward_substitution_cl(double r[]){
		for (int i = 0; i < rowSize ; i++){			
	
			list<node>::iterator it = m[i].begin();
			
			while(it->pos != i){
				r[i] -= it->value * r[it->pos];
				++it;
			}
			
			r[i] = r[i] / it->value;
		}
	}

	void SparseMatrix::eg(double r[]){
		for(int k = 0; k < rowSize - 1; k++)
			for(int i = k + 1; i < rowSize; i++)
					if(getFirstPos(i) == k){
						double m = getFirstVal(i) / getFirstVal(k);
						rowSub(k,i,m);
					}			
		backward_substitution(r);
	}
	

	SparseMatrix& SparseMatrix::cl(){
		SparseMatrix *lTrans =  new SparseMatrix(rowSize,colSize);
		 for(int j=0;j<rowSize;j++){
		 	double diagonal = dotProduct(j,j,1);
		 	node n = {diagonal,j}; 
		 	lTrans->addtoRow(j,n);
		 	for(int i = j+1; i<rowSize;i++){
		 		double val = dotProduct(i,j,diagonal);	
		 		node n2 = {val,i};
		 		lTrans->addtoRow(j,n2); 		
		 	}
		 }
		 return *lTrans;
	}
	
	void SparseMatrix::addtoRow(int row, node& n){
		m[row].push_back(n);
	}

	double  SparseMatrix::dotProduct(int i, int j, double diagonal){
		list<node>::iterator itRowi = m[i].begin();
		list<node>::iterator itRowj = m[j].begin();
		double sum = 0;
		while(itRowi != m[i].end() && itRowj != m[j].end() && itRowi->pos <= j-1 ){
			if (itRowi->pos == itRowj->pos){
				sum = sum+ itRowi->value* itRowj->value;
				++itRowj;
				++itRowi;	
			} 
			else if (itRowi->pos > itRowj->pos)
        		++itRowj;
        	else
        		++itRowi;
		}

		while(itRowi != m[i].end() && itRowi->pos <= j-1)
			++itRowi;
	
		if(itRowi != m[i].end() && itRowi->pos == j){
			sum = itRowi->value -sum;
			double val;
			if(j==i)
				val = pow(sum,1.0/2.0);
			else
				val = sum / diagonal;
			itRowi->value = val;
			return itRowi->value;
		}
		else{
			node n = node();
			if(i==j)
				n.value = pow((-1*sum),1.0/2.0);
			else
	        	n.value = (-1*sum)/diagonal;
	        n.pos = j;
			m[i].insert(itRowi,n);
			return n.value;
		}
	
	}




	bool SparseMatrix::isZero(double k){
		return (fabs(k) <= 1.0e-7);	
	}
	



// int main()
// {	
	
// 	int a [3][3];
// 	a[0][0] = 1;
// 	a[0][1] = 0;
// 	a[0][2] = 1;
// 	a[1][0] = 3;
// 	a[1][1] = 0;
// 	a[1][2] = 0;
// 	a[2][0] = 5;
// 	a[2][1] = 0;
// 	a[2][2] = 0;
// 	vector< vector<int> > matrix;
// 	cout << "Filling matrix with test numbers.";
// 	matrix.resize(3);  
// 	for (int i = 0; i < 3; i++)
// 	{
// 	    matrix[i].resize(3);  
// 	    for (int j = 0; j < 3; j++)
// 	    {
// 	        matrix[i][j] = a[i][j];  
// 	    }
// 	}
	
// 	SparseMatrix m = SparseMatrix(matrix);

//     std::cout << "mylist contains:"<<endl;
//     m.show();
//    /* m.scalarMult(2);
//   	m.show();
//   	m.ScalarRowMult(2,1);
//   	m.show();
//   	m.ScalarRowMult(2,0);
//     m.show();
//     m.rowSub(0,1,3);
//     m.show();
//     m.scalarMult(3);
//   	m.show();
// 	*/
// 	m.rowSub(0,1,3);
// 	m.show();
// 	m.rowSub(0,2,5);
// 	m.show();
// 	float c = float(5) / float(3);
// 	m.rowSub(1,2,c);
// 	m.show();
//     return 0;

// }
