#!/usr/bin/env python
import os
import sys

n= int(sys.argv[1]);
m= int(sys.argv[2]);
for i in range(n,m):
	os.system('python generarMatriz.py '+str(i));
	for j in range(0,3):	
		os.system('./a.out test_'+str(i)+'_1.in out/test_'+str(i)+'_1_'+str(j)+'.out '+str(j)+' 5000');
os.system('python graficosTiempo.py');

