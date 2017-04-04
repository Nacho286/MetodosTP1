#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

int equipos; // La declaro global para poder usarla en las demas funciones

void eg(vector< vector<double> > &matriz, double r[]){
	//Eliminacion Gaussiana
	for(int k = 0; k < equipos - 1; k++){
		for(int i = k + 1; i < equipos; i++){
				double m = matriz[i][k] / matriz[k][k];
				for(int j = k; j < equipos + 1; j++)
					matriz[i][j] -= m * matriz[k][j];
		}
	}
}

void cl(vector< vector<double> > &matriz, double r[]){
	//Cholesky
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

int main (int args, char* argsv[]) {
	ifstream entrada;
	entrada.open(argsv[1]);
	string cabecera;
	getline(entrada, cabecera);	// La primera linea contiene la cantidad de equipos y partidos
	vector<string> tamanio = split(cabecera, ' ');

	//char modo = stoi(argsv[3]); // 0 EG, 1 CL, 2 WP
	char modo = 0;

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

	//MEDIR_TIEMPO(START)
	if (modo == 0)
		eg(matriz, r);
	else if (modo == 1)
		cl(matriz, r);
	else
		wp(total, r);
	//MEDIR_TIEMPO(END)

	// Los arreglos se pasan como punteros, r contiene el resultado
	// ofstream salida;
	// salida.open(argsv[2]);
	// // Se respeta el orden de los equipos en el archivo de salida
	// for (j = 0; j < equipos; j++)
	// 	salida << to_string(r[j]) << "\n";
	// salida.close();

	imprimir(matriz);

	return 0;
}