#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cmath>
#include "tiempo.h"
#include "SparseMatrix.h"
using namespace std;

int equipos; // La declaro global para poder usarla en las demas funciones

bool isZero(double value){
	return (abs(value) <= 1.0e-7);
}

void backward_substitution(vector< vector<double> > &matriz, double r[]){
	for (int i = equipos - 1; i >= 0; i--){
		r[i] = matriz[i][equipos];			// r_i = b_i
		for (int j = equipos - 1; j >= i + 1; j--)
			r[i] -= matriz[i][j] * r[j];
		r[i] = r[i] / matriz[i][i];
	}
}

void forward_substitution(vector< vector<double> > &matriz, double r[]){
	for (int i =0; i < equipos; i++){
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
	matriz[0][0]=pow(matriz[0][0],1.0/2.0);
	for(int i=1;i<equipos;i++){
        matriz[i][0]=matriz[i][0]/matriz[0][0];
	}
	for(int i=1;i<equipos-1;i++){
        for(int j=0;j<i;j++){
            matriz[i][i]-=pow(matriz[i][j],2);

        }
        matriz[i][i]=pow(matriz[i][i],1.0/2.0);
        for(int j=i+1;j<equipos;j++){
            for(int k=0;k<i;k++){
                matriz[j][i]-=(matriz[j][k]*matriz[i][k]);
            }
            matriz[j][i]=matriz[j][i]/matriz[i][i];
        }
	}
	for(int i=0;i<equipos-1;i++){
        matriz[equipos-1][equipos-1]-=pow(matriz[equipos-1][i],2);
	}
	matriz[equipos-1][equipos-1]=pow(matriz[equipos-1][equipos-1],1.0/2.0);
	for(int i=0;i<equipos;i++){
		for(int j=0;j<equipos;j++){
			matriz[i][j]=matriz[j][i];
		}
	}
}


void solve_cl(vector< vector<double> > &matriz, double r[]){
	forward_substitution(matriz,r);
	for(int i=0;i<equipos;i++){
		matriz[i][equipos]=r[i];
	}
	backward_substitution(matriz,r);
}

void solve_cl(vector< vector<double> > &matriz, double r[],vector< vector<double>> bs){
	for(vector<double> b: bs){
		for(int i=0;i<equipos;i++){
			matriz[i][equipos]=b[i];
		}
		forward_substitution(matriz,r);
		for(int i=0;i<equipos;i++){
			matriz[i][equipos]=r[i];
		}
		backward_substitution(matriz,r);
	}
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

template<typename Out>
void split(const string &s, char delim, Out result) {
    stringstream ss;
    ss.str(s);
    string item;
    while (getline(ss, item, delim)) {
        *(result++) = item;
    }
}


vector<string> split(const string &s, char delim) {
    vector<string> elems;
    split(s, delim, back_inserter(elems));
    return elems;
}

void procesar_mediciones(unsigned long long resultados[], unsigned long long res, int iteraciones){
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
	string file_name = "medicion." + to_string(equipos) + ".txt";
	ofstream medicion;
	medicion.open(file_name, ios::out | ios::app);
	medicion << "------------------------------------------------------\n";
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

	ifstream entrada;
	entrada.open(argsv[1]);
	string cabecera;
	getline(entrada, cabecera);	// La primera linea contiene la cantidad de equipos y partidos
	vector<string> tamanio = split(cabecera, ' ');

	char modo = stoi(argsv[3]); // 0 EG, 1 CL, 2 WP

	equipos = stoi(tamanio[0]);
	int partidos = stoi(tamanio[1]);
	vector< vector<double> > matriz(equipos, vector<double>(equipos + 1));

	if (modo != 2){
		//Inicializar matriz con 2 en la diagonal (menos ultima columna)
		// y 1 en la ultima columna
		for (int j = 0; j < equipos; j++){
			for (int k = 0; k < equipos + 1; k++){
				if (j == k && k < equipos)
					matriz[j][k] = 2.0;
				if (k == equipos)
					matriz[j][k] = 1.0;
			}
		}
	}

	string linea;
	double r[equipos];
	double total[equipos]; // Para caso WP
	int i = 0;
	while (i < partidos){
		getline(entrada, linea);
		vector<string> resultado = split(linea, ' '); //Formato: fecha e_1 r_1 e_2 r_2
		int equipo_1    = stoi(resultado[1]);
		int resultado_1 = stoi(resultado[2]);
		int equipo_2    = stoi(resultado[3]);
		int resultado_2 = stoi(resultado[4]);

		if (modo != 2){ //Armo matriz CMM
			matriz[equipo_1 - 1][equipo_2 - 1] -= 1.0; // Partido entre ellos
			matriz[equipo_2 - 1][equipo_1 - 1] -= 1.0;
			matriz[equipo_1 - 1][equipo_1 - 1] += 1.0; // Partidos totales
			matriz[equipo_2 - 1][equipo_2 - 1] += 1.0;

			//Dejo el caso de empate por si surge en nuestros tests generados automáticamente
			//Esos tests son de mediciones de tiempo, asi que no importa quien quede como ganador
			if (resultado_1 >= resultado_2){
				matriz[equipo_1 - 1][equipos] += 1.0/2.0;
				matriz[equipo_2 - 1][equipos] -= 1.0/2.0;
			}else{
				matriz[equipo_1 - 1][equipos] -= 1.0/2.0;
				matriz[equipo_2 - 1][equipos] += 1.0/2.0;
			}
		}else{ // Caso WP
			if (resultado_1 >= resultado_2)
				r[equipo_1 - 1] += 1.0;
			else
				r[equipo_2 - 1] += 1.0;

			total[equipo_1 - 1] += 1.0;
			total[equipo_2 - 1] += 1.0;
		}
		i++;
	}
	entrada.close();

	int iteraciones;
	if (args > 4) //Hay parametro cant. de ejecuciones	
		iteraciones = stoi(argsv[4]);
	else
		iteraciones = 1;

	cout << "Rala:"<<endl;
	SparseMatrix rala = SparseMatrix(matriz, equipos,equipos);
	rala.show();
	rala.cl();
	rala.show();
	
	unsigned long long start, end, res;
	unsigned long long resultados[iteraciones];
	for (int k = 0; k < iteraciones; k++){
		MEDIR_TIEMPO_START(start)
		if (modo == 0){
			rala.eg(r);		
		}
		else if (modo == 1){
			cl(matriz);
			//solve_cl(matriz,r);
		}
		else
			wp(total, r);
		MEDIR_TIEMPO_STOP(end)
		resultados[k] = end - start;
		res += end - start;
	}
	
	if (iteraciones > 1)
		procesar_mediciones(resultados, res, iteraciones);

	// Los arreglos se pasan como punteros, r contiene el resultado
	ofstream salida;
	salida.open(argsv[2]);
	// Se respeta el orden de los equipos en el archivo de salida
	for (int j = 0; j < equipos; j++)
	 	salida << r[j] << "\n";
	salida.close();

	cout << "Comun:"<<endl;
	//if (modo != 2)
	 	imprimir(matriz);

	return 0;
}
