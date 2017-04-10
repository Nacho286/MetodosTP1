#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cmath>
#include "tiempo.h"
#include "SparseMatrix.h"
#include "archivos.h"


int main (int args, char* argsv[]) {

	int equipos = archivos::cantEquipos(argsv[1]);

	double r[equipos];
	double r2[equipos];

	double total[equipos]; // Para caso WP
	vector< vector<double> > matriz;

	// Para EG, uso una matriz de n x (n + 1) (la ultima columna es el b)
	matriz.resize(equipos, vector<double>(equipos + 1));
	matriz = archivos::armar_matriz_eg(argsv[1]);
	archivos::armar_wp(argsv[1], r2, total);
	
	SparseMatrix rala = SparseMatrix(matriz, equipos, equipos+1);
	rala.eg(r);		

	// Los arreglos se pasan como punteros, r contiene el resultado
	ofstream salida;
	salida.open(argsv[2]);
	// Se respeta el orden de los equipos en el archivo de salida
	for (int j = 0; j < equipos; j++)
	 	salida << r[j] << "\n";
	//salida.close();

	for (int i = 0; i < equipos; i++)
		r2[i] = r2[i] / total[i];

	for (int j = 0; j < equipos; j++)
	 	salida << r2[j] << "\n";
	salida.close();


	return 0;
}