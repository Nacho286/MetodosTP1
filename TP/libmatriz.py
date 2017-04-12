import os
import numpy as np
import random as ran

def generar_Colley(n, m):
	colley = np.zeros((n, n))
	total = 0
	for i in xrange(n):
		n_i = 2
		for j in xrange(n): 	# 3 partidos jugados entre equipos como maximo
			if(j>i):
				n_ij = ran.randint(0, 3)
				colley[i][j] = (-1) * n_ij
				colley[j][i] = colley[i][j]
				n_i += n_ij
			else:
				n_i += abs(colley[i][j])
		colley[i][i] = n_i
		total += n_i - 2
	print colley
	generar_partidos(colley, n, m, total)

def generar_partidos(matriz, n, m, partidos):
	for k in xrange(1, m + 1):
		archivo = "test_" + str(n) + "_" + str(k) + ".in"
		b = open(archivo, "w")
		b.write(str(n) + " " + str(partidos) + "\n")
		for i in xrange(n):
			for j in xrange(i + 1, n):
				n_ij = int((-1) * matriz[i][j])
				if n_ij != 0:
					for h in xrange(n_ij):
						res_1 = ran.randint(0, 1)
						res_2 = 1 - res_1
						b.write("1 " + str(i + 1) + " " + str(res_1) + " " + str(j + 1) + " " + str(res_2) + "\n")
		b.close()
