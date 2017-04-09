using namespace std;

namespace archivos{
	template<typename Out>
	void split(const string &s, char delim, Out result) {
    	stringstream ss;
    	ss.str(s);
    	string item;
    	while (getline(ss, item, delim))
        	*(result++) = item;
	}	


	vector<string> split(const string &s, char delim) {
    	vector<string> elems;
    	split(s, delim, back_inserter(elems));
    	return elems;
	}

	int cantEquipos(string file){
		ifstream entrada;
		entrada.open(file);
		string cabecera;
		getline(entrada, cabecera);
		vector<string> tamanio = split(cabecera, ' ');
		entrada.close()
		return stoi(tamanio[0]);
	}

	vector< vector<double> > armar_matriz_eg(string file){
		ifstream entrada;
		entrada.open(file);
		string cabecera;
		getline(entrada, cabecera);	// La primera linea contiene la cantidad de equipos y partidos
		vector<string> tamanio = split(cabecera, ' ');
		int equipos  = stoi(tamanio[0]);
		int partidos = stoi(tamanio[1]);
		vector< vector<double> > matriz(equipos, vector<double>(equipos + 1));

		for (int j = 0; j < equipos; j++){
			for (int k = 0; k < equipos + 1; k++){
				if (j == k && k < equipos)
					matriz[j][k] = 2.0;
				if (k == equipos)
					matriz[j][k] = 1.0;
			}
		}

		string linea;
		int i = 0;
		while (i < partidos){
			getline(entrada, linea);
			vector<string> resultado = split(linea, ' '); //Formato: fecha e_1 r_1 e_2 r_2
			int equipo_1    = stoi(resultado[1]);
			int resultado_1 = stoi(resultado[2]);
			int equipo_2    = stoi(resultado[3]);
			int resultado_2 = stoi(resultado[4]);

			matriz[equipo_1 - 1][equipo_2 - 1] -= 1.0; // Partido entre ellos
			matriz[equipo_2 - 1][equipo_1 - 1] -= 1.0;
			matriz[equipo_1 - 1][equipo_1 - 1] += 1.0; // Partidos totales
			matriz[equipo_2 - 1][equipo_2 - 1] += 1.0;

			if (resultado_1 > resultado_2){
				matriz[equipo_1 - 1][equipos] += 1.0/2.0;
				matriz[equipo_2 - 1][equipos] -= 1.0/2.0;
			}else{
				matriz[equipo_1 - 1][equipos] -= 1.0/2.0;
				matriz[equipo_2 - 1][equipos] += 1.0/2.0;
			}
			i++;
		}
		entrada.close();

		return matriz;
	}

	vector< vector<double> > armar_matriz_cl(string file, double r[]){
		ifstream entrada;
		entrada.open(file);
		string cabecera;
		getline(entrada, cabecera);	// La primera linea contiene la cantidad de equipos y partidos
		vector<string> tamanio = split(cabecera, ' ');
		int equipos  = stoi(tamanio[0]);
		int partidos = stoi(tamanio[1]);
		vector< vector<double> > matriz(equipos, vector<double>(equipos));

		for (int j = 0; j < equipos; j++){
			r[j] = 1.0;
			for (int k = 0; k < equipos; k++){
				if (j == k)
					matriz[j][k] = 2.0;
			}
		}

		string linea;
		int i = 0;
		while (i < partidos){
			getline(entrada, linea);
			vector<string> resultado = split(linea, ' '); //Formato: fecha e_1 r_1 e_2 r_2
			int equipo_1    = stoi(resultado[1]);
			int resultado_1 = stoi(resultado[2]);
			int equipo_2    = stoi(resultado[3]);
			int resultado_2 = stoi(resultado[4]);

			matriz[equipo_1 - 1][equipo_2 - 1] -= 1.0; // Partido entre ellos
			matriz[equipo_2 - 1][equipo_1 - 1] -= 1.0;
			matriz[equipo_1 - 1][equipo_1 - 1] += 1.0; // Partidos totales
			matriz[equipo_2 - 1][equipo_2 - 1] += 1.0;

			if (resultado_1 > resultado_2){
				r[equipo_1 - 1] += 1.0/2.0;
				r[equipo_2 - 1] -= 1.0/2.0;
			}else{
				r[equipo_1 - 1] -= 1.0/2.0;
				r[equipo_2 - 1] += 1.0/2.0;
			}
			i++;
		}
		entrada.close();

		return matriz;
	}

	void armar_wp(string file, double r[], double total[]){
		ifstream entrada;
		entrada.open(file);
		string cabecera;
		getline(entrada, cabecera);	// La primera linea contiene la cantidad de equipos y partidos
		vector<string> tamanio = split(cabecera, ' ');
		int equipos  = stoi(tamanio[0]);
		int partidos = stoi(tamanio[1]);

		string linea;
		int i = 0;
		while(i < partidos){
			getline(entrada, linea);
			vector<string> resultado = split(linea, ' '); //Formato: fecha e_1 r_1 e_2 r_2
			int equipo_1    = stoi(resultado[1]);
			int resultado_1 = stoi(resultado[2]);
			int equipo_2    = stoi(resultado[3]);
			int resultado_2 = stoi(resultado[4]);

			if (resultado_1 >= resultado_2)
				r[equipo_1 - 1] += 1.0;
			else
				r[equipo_2 - 1] += 1.0;

			total[equipo_1 - 1] += 1.0;
			total[equipo_2 - 1] += 1.0;
		}
		entrada.close()		
	}

	void obtener_b(string file, double r[]){
		ifstream entrada;
		entrada.open(file);
		string cabecera;
		getline(entrada, cabecera);	// La primera linea contiene la cantidad de equipos y partidos
		vector<string> tamanio = split(cabecera, ' ');
		int equipos  = stoi(tamanio[0]);
		int partidos = stoi(tamanio[1]);

		for (int j = 0; j < equipos; j++)
			r[j] = 1.0;

		string linea;
		int i = 0;
		while (i < partidos){
			getline(entrada, linea);
			vector<string> resultado = split(linea, ' '); //Formato: fecha e_1 r_1 e_2 r_2
			int equipo_1    = stoi(resultado[1]);
			int resultado_1 = stoi(resultado[2]);
			int equipo_2    = stoi(resultado[3]);
			int resultado_2 = stoi(resultado[4]);

			if (resultado_1 > resultado_2){
				r[equipo_1 - 1] += 1.0/2.0;
				r[equipo_2 - 1] -= 1.0/2.0;
			}else{
				r[equipo_1 - 1] -= 1.0/2.0;
				r[equipo_2 - 1] += 1.0/2.0;
			}
			i++;
		}
		entrada.close();	
	}

	void procesar_mediciones(unsigned long long resultados[], unsigned long long res, int iteraciones, char b, char modo){
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
		medicion << "Instancia (b): " << b << "\n";
		medicion << "Promedio: " << media << "\n";
		medicion << "Desviacion standar: " << sd << "\n";
		medicion << "#Iteraciones: " << iteraciones << "\n";
		medicion << "#Elem. removidos: " << h << "\n";
		medicion.close();
	}
}