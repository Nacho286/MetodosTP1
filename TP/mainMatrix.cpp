#include <iostream>
#include <list>
#include <vector>
#include <cstdlib>
 #include <math.h>
using namespace std;

class SparseMatrix{


private:

	struct node{
		double value; 
		int pos;
	};
	std::vector<std::list<node> > m;
	int size;

public:

	//Constructor 
	SparseMatrix(std::vector<std::vector<int> > a) {
	    for(int i = 0; i< a.size(); i++){
	        std::list<node>row;
	        for(int j=0; j< a.size(); j++){
	            if(!isZero(a[i][j])){
	            	node * n = new node;
	            	n->value = a[i][j];
	            	n->pos = j;
	                row.push_back(*n);
	            }    
	        }
	        m.push_back(row);
	    }
	    size = a.size();

	}

	int getSize(){
	    return size;
	}

	//Imprime la matriz
	void show(){

		for(int i = 0; i< size; i++){
	        list<node>::iterator row = m[i].begin();
	        for(int j=0; j< size; j++){

	            if(row->pos == j){
	            	cout << ' ' << row->value;
	            	++row;
	            }
	            else
	            	cout << ' ' << 0;
	               
	        }
	      cout << endl;
	    }
	    cout << endl;
		// vector< list<node> >::iterator row;
		// list<node>::iterator col;
		// for (row = m.begin(); row != m.end(); row++) {
		//     for (col = row->begin(); col != row->end(); col++) {
		//            cout << ' ' << col->value;
		//     }
		//     cout << endl;

		// }
	 //    cout << endl;
	}

	//Multiplica la matriz por un escalar (k*A, k un real) 
	void scalarMult(double a){
		vector< list<node> >::iterator row;
		list<node>::iterator col;
		for (row = m.begin(); row != m.end(); row++) {
		    for (col = row->begin(); col != row->end(); col++) 
		           col->value = col->value *a;
		}
	    
	}

	//Multiplica la fila row de m por el escalar a 
	void ScalarRowMult(double a, int row){
		list<node>::iterator itRow;
		for (itRow = m[row].begin(); itRow != m[row].end(); itRow++)
			itRow->value = itRow->value * a ;
		
	}

	//Resta fila lowerRow - m*pivotRow

	void rowSub(int pivotRow, int lowerRow, double k){

		//se copia la fila
		list<node> lpivotRow = m[pivotRow];

		//Inicializacion de iteradores
		list<node>::iterator itPivotRow = lpivotRow.begin();
		list<node>::iterator itLowerRow = m[lowerRow].begin();

		while(itPivotRow != lpivotRow.end() && itLowerRow != m[lowerRow].end()){
			if (itPivotRow->pos ==  itLowerRow->pos){
				itLowerRow->value -= (k*itPivotRow->value);

				if(isZero(itLowerRow->value))
					itLowerRow = m[lowerRow].erase(itLowerRow);
				else
					++itLowerRow;
            	++itPivotRow;
			}
        	else if (itLowerRow->pos > itPivotRow->pos){
        		node * n = new node;
	            n->value = -k*(itPivotRow->value);
	            n->pos = itPivotRow->pos;
        		m[lowerRow].insert(itLowerRow,*n);
        		++itPivotRow;
        	}
        	else
        		++itLowerRow;
		}
    	
   		while(itPivotRow != lpivotRow.end()){
   			node * n = new node;
	        n->value = -k*(itPivotRow->value);
	        n->pos = itPivotRow->pos;
        	m[lowerRow].insert(itLowerRow,*n);
        	++itPivotRow;
     	}
 
	}

	bool isZero(double k){
		return (fabs(k) <= 1.0e-5);	

	}
	

};


int main()
{	
	
	int a [3][3];
	a[0][0] = 1;
	a[0][1] = 0;
	a[0][2] = 1;
	a[1][0] = 3;
	a[1][1] = 0;
	a[1][2] = 0;
	a[2][0] = 5;
	a[2][1] = 0;
	a[2][2] = 0;
	vector< vector<int> > matrix;
	cout << "Filling matrix with test numbers.";
	matrix.resize(3);  
	for (int i = 0; i < 3; i++)
	{
	    matrix[i].resize(3);  
	    for (int j = 0; j < 3; j++)
	    {
	        matrix[i][j] = a[i][j];  
	    }
	}
	SparseMatrix m = SparseMatrix(matrix);

    std::cout << "mylist contains:"<<endl;
    m.show();
   /* m.scalarMult(2);
  	m.show();
  	m.ScalarRowMult(2,1);
  	m.show();
  	m.ScalarRowMult(2,0);
    m.show();
    m.rowSub(0,1,3);
    m.show();
    m.scalarMult(3);
  	m.show();
	*/
	m.rowSub(0,1,3);
	m.show();
	m.rowSub(0,2,5);
	m.show();
	float c = float(5) / float(3);
	m.rowSub(1,2,c);
	m.show();
    return 0;

}
