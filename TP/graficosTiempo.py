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
	print f;
 	archivo.readline();
	promedio= float(archivo.readline().split(':')[1]);
	nombre=f.split('.');	
	modo=nombre[1];
	equipos=nombre[2];
	if(maxEquipos<equipos):
		maxEquipos=equipos;
	if(maxPromedio<promedio):
		maxPromedio=promedio;
	if(modo=='eg'):
		eg[equipos]=promedio;
	elif(modo=='cl'):
		cl[equipos]=promedio;
	else:
		pw[equipos]=promedio;

plt.xlabel('Cantidad de equipos');
plt.ylabel('Tiempo promedio de duracion del progama');
plt.title('Duracion promedio del progama en funcion de la cantidad de equipos');
keys= eg.keys();
keys.sort;
plotPromedio=[];
for i in keys:
	plotPromedio.append(eg[i]);
plt.plot(keys,plotPromedio,label='Eliminacion gaussiana');

keys= cl.keys();
keys.sort;
plotPromedio=[];
for i in keys:
	plotPromedio.append(cl[i]);
plt.plot(keys,plotPromedio,label='Cholesky');

keys= pw.keys();
keys.sort;
plotPromedio=[];
for i in keys:
	plotPromedio.append(pw[i]);
plt.plot(keys,plotPromedio,label='pw');
print maxEquipos;
print maxPromedio;
plt.legend(bbox_to_anchor=(1.09,0.8));
#plt.axis([0,maxEquipos,0,maxPromedio]);
plt.show();
