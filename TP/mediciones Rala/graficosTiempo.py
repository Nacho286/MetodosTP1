#!/usr/bin/env python
import os
import numpy as np
import matplotlib.pyplot as plt

eg={};
cl={};
pw={};
files= [f for f in os.listdir('.') if os.path.isfile(f) if 'medicion.' in f]
maxEquipos=0;
maxPromedio=0;
for f in files:
	archivo=open(f,'r');
	print f;
 	archivo.readline();
	archivo.readline();
	promedio= float(archivo.readline().split(':')[1]);
	nombre=f.split('.')[1];
	
	nombre=nombre.split('_');	
	modo=nombre[1];
	print modo;
	equipos=int(nombre[0]);
	print equipos;	
	if(maxEquipos<equipos):
		maxEquipos=equipos;
	if(maxPromedio<promedio):
		maxPromedio=promedio;
	if(modo=='EG'):
		eg[equipos]=promedio;
	elif(modo=='CL'):
		cl[equipos]=promedio;
	elif(modo=='WP'):
		pw[equipos]=promedio;

plt.xlabel('Cantidad de equipos');
plt.ylabel('Tiempo promedio de duracion del progama');
plt.title('Duracion promedio del progama en funcion de la cantidad de equipos(mseg)');
keys= eg.keys();
keys.sort();
print keys;
plotPromedio=[];
for i in keys:	
	plotPromedio.append(eg[i]);
plt.plot(keys,plotPromedio,label='Eliminacion gaussiana');

keys= cl.keys();
keys.sort();
plotPromedio=[];
print keys;
for i in keys:
	
	plotPromedio.append(cl[i]);
plt.plot(keys,plotPromedio,label='Cholesky');

keys= pw.keys();
keys.sort();
print keys;
plotPromedio=[];
for i in keys:
	plotPromedio.append(pw[i]);
plt.plot(keys,plotPromedio,label='pw');
print maxEquipos;
print maxPromedio;


plt.legend(bbox_to_anchor=(0.6,0.9));
plt.axis([0,maxEquipos,0,maxPromedio]);
plt.show();


