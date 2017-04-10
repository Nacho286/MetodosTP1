#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cmath>
#include "tiempo.h"
#include "SparseMatrix.h"
#include "archivos.h"

int equipos; // La declaro global para poder usarla en las demas funciones

bool isZero(double value){
	return (abs(value) <= 1.0e-7);
}

//EG: b es la ultima columna de la matriz
void backward_substitution(vector< vector<double> > &matriz, double r[]){
	for (int i = equipos - 1; i >= 0; i--){
		r[i] = matriz[i][equipos];			// r_i = b_i
		for (int j = equipos - 1; j >= i + 1; j--)
			r[i] -= matriz[i][j] * r[j];
		r[i] = r[i] / matriz[i][i];
	}
}

//EG: b es la ultima columna de la matriz
void forward_substitution(vector< vector<double> > &matriz, double r[]){
	for (int i = 0; i < equipos; i++){
		r[i] = matriz[i][equipos];			// r_i = b_i
		for (int j = 0; j < i ; j++)
			r[i] -= matriz[i][j] * r[j];
		r[i] = r[i] / matriz[i][i];
	}
}

void eg(vector< vector<double> > &matriz, double r[]){
	//Eliminacion Gaussiana
	for(int k = 0; k < equipos - 1; k++){
		for(int i = k + 1; i < equipos; i++){
				double m = matriz[i][k] / matriz[k][k];
				for(int j = k; j < equipos + 1; j++){
					matriz[i][j] -= m * matriz[k][j];
					if (isZero(matriz[i][j]))
						matriz[i][j] = 0.0;
				}
		}
	}
	backward_substitution(matriz, r);
}

void cl(vector< vector<double> > &matriz){
	//Cholesky
	matriz[0][0] = pow(matriz[0][0], 1.0/2.0);
	for(int i = 1;i < equipos; i++)
        matriz[i][0] = matriz[i][0] / matriz[0][0];

	for(int i = 1;i < equipos - 1; i++){
        for(int j = 0; j < i; j++)
            matriz[i][i] -= pow(matriz[i][j], 2);
        matriz[i][i] = pow(matriz[i][i], 1.0/2.0);

        for(int j= i + 1; j < equipos; j++){
            for(int k = 0; k < i; k++)
                matriz[j][i] -= (matriz[j][k] * matriz[i][k]);
            matriz[j][i] = matriz[j][i] / matriz[i][i];
        }
	}
	for(int i = 0; i < equipos - 1; i++)
        matriz[equipos - 1][equipos - 1] -= pow(matriz[equipos - 1][i], 2);
	matriz[equipos - 1][equipos - 1] = pow(matriz[equipos - 1][equipos - 1], 1.0/2.0);

	for(int i = 0;i < equipos; i++){
		for(int j = 0; j < equipos; j++)
			matriz[i][j] = matriz[j][i];
	}
}


void solve_cl(vector< vector<double> > &matriz, double r[]){
	forward_substitution(matriz, r);
	for(int i = 0; i < equipos; i++)
		matriz[i][equipos] = r[i];
	backward_substitution(matriz, r);
}

void solve_cl(SparseMatrix &l, SparseMatrix &lt, double r[]){
		l.forward_substitution_cl(r);
		lt.backward_substitution_cl(r);
}


void wp(double total[], double r[]){
	//Win Percentage
	// r_i tiene la cant. de partidos ganados del equipo i
	// total_i tiene el total de partidos jugados del equipo i
	for (int i = 0; i < equipos; i++)
		r[i] = r[i] / total[i];
}

void imprimir(vector< vector<double> > matriz){
	for (int i = 0; i < equipos; i++){
		for (int j = 0; j <= equipos; j++){
			cout << matriz[i][j] << " ";
		}
		cout << "\n";
	}
}

void procesar_mediciones(unsigned long long resultados[], unsigned long long res, int iteraciones, int b, int modo){
		const float z_90 = 1.282;
		const float z_10 = -1.282;
		double media, varianza, sd, sumatoria, x_90, x_10;
		media = res / iteraciones;
		for (int i = 0; i < iteraciones; i++)
			sumatoria += (resultados[i] - media) * (resultados[i] - media);
		varianza = sumatoria / (double) iteraciones;
		sd = sqrt(varianza);
		x_90 = media + z_90 * sd;
		x_10 = media + z_10 * sd;
		int h = 0;
		// cuento la cant de elementos a remover
		for (int j = 0; j < iteraciones; j++) {
			if (resultados[j] > x_90 || resultados[j] < x_10)
				h++;
		}
		int n = iteraciones - h;
		unsigned long long mediciones[n];
		res = 0;
		for (int j = 0; j < iteraciones; j++){
			if (!(resultados[j] >  x_90) || !(resultados[j] < x_10)){
				mediciones[j] = resultados[j];
				res += mediciones[j];
			}
		}
		media = res / n;
		string metodo;
		switch (modo){
			case 0:
				metodo = "EG";
				break;
			case 1:
				metodo = "CL";
				break;
			case 2:
				metodo = "WP";
				break;
		}
		string file_name = "medicion." + to_string(equipos) + "_" + metodo + ".txt";
		ofstream medicion;
		medicion.open(file_name, ios::out | ios::app);
		medicion << "------------------------------------------------------\n";
		medicion << "Instancia (b): " << to_string(b) << "\n";
		medicion << "Promedio: " << media << "\n";
		medicion << "Desviacion standar: " << sd << "\n";
		medicion << "#Iteraciones: " << iteraciones << "\n";
		medicion << "#Elem. removidos: " << h << "\n";
		medicion.close();
	}

int main (int args, char* argsv[]) {
	if (args < 4){
		cout << "Argumentos inválidos (archivo_entrada archivo_salida 0/1/2)" << "\n";
		return 1;
	}

	int modo = stoi(argsv[3]); // 0 EG, 1 CL, 2 WP

	equipos = archivos::cantEquipos(argsv[1]);
	double r[equipos];
	double total[equipos]; // Para caso WP
	vector< vector<double> > matriz;
	switch (modo){
		case 0:
			// Para EG, uso una matriz de n x (n + 1) (la ultima columna es el b)
			matriz.resize(equipos, vector<double>(equipos + 1));
			matriz = archivos::armar_matriz_eg(argsv[1]);
			break;
		case 1:
			// Para CL, uso un arreglo separado para el b
			matriz.resize(equipos, vector<double>(equipos));
			matriz = archivos::armar_matriz_cl(argsv[1], r);
			break;
		case 2:
			archivos::armar_wp(argsv[1], r, total);
			break;
	}

	int iteraciones;
	if (args > 4) //Hay parametro cant. de ejecuciones	
		iteraciones = stoi(argsv[4]);
	else
		iteraciones = 1;

	int n = equipos;
	int m = equipos;
	if (modo == 0){
		m++;
	} else if (modo == 2){	// Necesario para que el constructor de SparseMatrix no itere sobre una matriz vacia
		n = 0;
		m = 0;
	}
	SparseMatrix rala = SparseMatrix(matriz, n, m);
	SparseMatrix l_t  = SparseMatrix(0, 0);			// La instancio ahora porque necesito reutilizarla
	
	int bs;
	if (args > 5) // Hay parametro cant. de b
		bs = stoi(argsv[5]);
	else
		bs = 1;
	// Si el modo es 2, no usar parametro b
	if (modo == 2)
		bs = 1;

	unsigned long long start, end, res;
	unsigned long long resultados[iteraciones];
	double copia_r[equipos];
	for (int h = 0; h < bs; h++){ 			// Utilizado para medir los tiempos frente a misma matriz con distintos b
		if (h > 0){
			string file = "test_" + to_string(equipos) + "_" + to_string(h + 1) + ".in";
			if (modo == 0){
				matriz = archivos::armar_matriz_eg(file);
				rala = SparseMatrix(matriz, n, m);
			}else
				archivos::obtener_b(file, r);
		}
		res = 0;
		start = 0;
		end = 0;
		for (int k = 0; k < iteraciones; k++){
			if (k == 0)	
				copy(r, r + equipos, copia_r);	
			copy(copia_r, copia_r + equipos, r);	// Restauro el b original, se modifica en cada iteración
			// Habria que hacer lo mismo con la matriz en el caso de EG
			MEDIR_TIEMPO_START(start)
			if (modo == 0)
				eg(matriz, r);
				//rala.eg(r);		
			else if (modo == 1){
				if (h == 0)
					cl(matriz);
					//l_t = rala.cl();
				//solve_cl(rala, l_t, r);
				solve_cl(matriz, r);
			}else
				wp(total, r);
			MEDIR_TIEMPO_STOP(end)
			resultados[k] = end - start;
			res += end - start;
		}
		if (iteraciones > 1)
			procesar_mediciones(resultados, res, iteraciones, h + 1, modo);
	}

	// Los arreglos se pasan como punteros, r contiene el resultado
	ofstream salida;
	salida.open(argsv[2]);
	// Se respeta el orden de los equipos en el archivo de salida
	for (int j = 0; j < equipos; j++)
	 	salida << r[j] << "\n";
	salida.close();

	return 0;
}
