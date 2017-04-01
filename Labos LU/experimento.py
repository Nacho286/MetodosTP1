import matlab.engine as me
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
import numpy as np
import math
eng = me.start_matlab()

n = 30
repet = 20
pot = 14
cant = 2**pot

def quitar_outliers(data, m=2):
    media = np.mean(data)
    dev = np.std(data)
    return [x for x in data if abs(x - media) <= m*dev]

def aplastar_tiempos(experimentos):
    # `experimentos` es una lista de listas tal que`experimentos[i]` son los 
    #   tiempos de la i-esima corrida del experimento
    # `aplastar_tiempos` devuelve una lista (`res`) con los experimentos
    #   aplastados en una unica lista, luego de haber quitado los outliers
    aplastados = []
    largo = len(experimentos[0])
    for idx in xrange(largo):
        data = [x[idx] for x in experimentos]
        data = quitar_outliers(data)
        aplastados.append(np.mean(data))

    return aplastados

def main():
    # Hago varias corridas con la misma cantidad de sistemas para despues sacar
    #   outliers y tener un valor mas realista
    # En las siguientes listas voy a guardar las distintas repeticiones de cada experimento
    lista_tiempos_gauss = []
    lista_tiempos_lu = []
    lista_init_lu = []
    for rep in xrange(repet):
        print('Repeticion ' + str(rep))
        tiempos_gauss, init_lu, tiempos_lu = eng.tomar_tiempos(n, cant, nargout=3)
        # El llamado a Matlab devuelve un arreglos de arreglos si hay mas de un
        #   valor. Me quedo con la unica fila que tiene.
        tiempos_gauss = list(tiempos_gauss[0])
        tiempos_lu= list(tiempos_lu[0])

        lista_tiempos_gauss.append(tiempos_gauss) 
        lista_init_lu.append([init_lu])
        lista_tiempos_lu.append(tiempos_lu) 

    t_g = aplastar_tiempos(lista_tiempos_gauss)
    i_lu = aplastar_tiempos(lista_init_lu)[0]
    t_lu = aplastar_tiempos(lista_tiempos_lu)

    # Creo una lista que tiene tal que en la i-esima posicion tiene la suma de
    #   los tiempos de las primeras i resoluciones de sistemas resueltos con 
    #   eliminacion Gaussiana
    t_g_acumulados = []
    s = 0
    k = 0
    for i in range(cant):
        s += t_g[i]
        if i == 2**k-1:
            k += 1
            t_g_acumulados.append(s)

    # Creo una lista que tiene tal que en la i-esima posicion tiene la suma de
    #   los tiempos de las primeras i resoluciones de sistemas resueltos con 
    #   factoriazacion LU
    # Tengo que tener en cuenta el tiempo inicial comun de inicializar L y U
    t_lu_acumulados = []
    s = i_lu
    k = 0
    for i in range(cant):
        s += t_lu[i]
        if i == 2**k-1:
            k += 1
            t_lu_acumulados.append(s)

    # Grafico los resultados
    fig = plt.figure()

    plt.plot(t_g_acumulados, label='Eliminacion gaussiana')
    plt.plot(t_lu_acumulados, label='Factorizacion LU')

    ax = plt.gca()
    a = ax.get_xticks().tolist()
    a = ["2^"+str(int(x)) for x in a]
    ax.set_xticklabels(a)
    ax.tick_params(labelsize=20)
    plt.legend(loc=2, prop={'size':17})

    fig.suptitle('Tiempo en funcion de la cantidad de sistemas resueltos', fontsize=30)
    plt.xlabel('Cantidad de sistemas resueltos (en cantidad)', fontsize=25)
    plt.ylabel('Tiempo total (en segundos)', fontsize=25)

    plt.yscale('log')

    plt.show()

main()
