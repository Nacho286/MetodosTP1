#!/usr/bin/env python
import os
import matplotlib.pyplot as plt

eg={};
cl={};
pw={};
files= [f for f in os.listdir('.') if os.path.isfile(f) if 'medicion' in f]
maxEquipos=0;
maxPromedio=0;
for f in files:
    archivo=open(f,'r');
    archivo.readline();
    archivo.readline();
    print(archivo);
    promedio= float(archivo.readline().split(':')[1]);
    nombre=f.split('.')[1].split('_');
    modo=nombre[1];
    equipos=int(nombre[0]);
    if(maxEquipos<equipos):
        maxEquipos=equipos;
    if(maxPromedio<promedio):
        maxPromedio=promedio;
    if(modo=='EG'):
        eg[equipos]=promedio;
    elif(modo=='CL'):
        cl[equipos]=promedio;

plt.xlabel('Cantidad de equipos');
plt.ylabel('Tiempo promedio de duracion del progama');
plt.title('Duracion promedio del progama en funcion de la cantidad de equipos(mseg)');
keys= list(eg.keys());
keys.sort();
plotPromedio=[];
for i in keys:
	plotPromedio.append(eg[i]);
plt.plot(keys,plotPromedio,label='Eliminacion gaussiana');

keys= list(cl.keys());
keys.sort();
plotPromedio=[];
for i in keys:
	plotPromedio.append(cl[i]);
plt.plot(keys,plotPromedio,label='Cholesky');


plt.legend(bbox_to_anchor=(1.09,0.8));
plt.axis([0,maxEquipos,0,maxPromedio]);
plt.show();
