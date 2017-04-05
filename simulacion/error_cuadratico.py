from json import load
import numpy as np
import numpy.matlib as mat
from numpy.linalg import inv
from numpy import transpose as trans
import time
start_time = time.time()

#Formato  del JSON:
# [lat, lon, ax, ay, vx_gps, vy_gps, orientacion, checkpoint, time]

checkpoint = 0
it_checkpoint = 0
checkpoint_loc = [] # Coordenadas de los checkpoints
tam = 6
error = []
error_cuadratico_minimo = 10000


H_best = []
Q_best = []
R_best = []

# Matrices de Kalman
H = mat.identity(tam)
Q = mat.zeros((tam, tam))
R = mat.zeros((tam, tam))
K = mat.zeros((tam, tam))
P = mat.zeros((tam, tam))
P_estimada = mat.identity(tam)
F = mat.identity(tam)

# Vectores de Kalman
x = np.zeros(shape=(tam,1))
x_estimada = np.zeros(shape=(tam,1))
z = np.zeros(shape=(tam,1))

with open('medidas.json') as json_data:
    d = load(json_data)

with open('valores_matriz.json') as datos:
    valores = load(datos)

    for it in valores:
        # Damos los valores apropiados
        R[0,0] = it[0];
        R[1,1] = it[0];
        R[2,2] = it[1];
        R[3,3] = it[1];
        R[4,4] = it[2];
        R[5,5] = it[2];
       
        delta_t = 0.0
        t_inicio = 0.0 

        for linea in d:

            z[0] = linea[0] #lat
            z[1] = linea[1] #lon
            z[2] = linea[2] #ax
            z[3] = linea[3] #ay
            # z[4] = linea[2] #vx
            # z[5] = linea[3] #vy

        	# Implementar Kalman
            # x = F * x_ant
            # x[0] = linea[5] #px
            # x[1] = linea[6] #py
            # x[2] = linea[7] #ax
            # x[3] = linea[8] #ay
            # x[4] = linea[9] #vx
            # x[5] = linea[10] #vy
            
            delta_t = linea[4] - t_inicio
            t_inicio = linea[4] 

            F[0,2] = 0.5 * delta_t ** 2
            F[0,4] = delta_t
            F[1,3] = 0.5 * delta_t ** 2
            F[1,5] = delta_t
            F[4,2] = delta_t
            F[5,3] = delta_t 
             
            try:
                x = F * x_estimada 
                P = F * P_estimada * trans(F) + Q
                K = P * trans(H) * inv(H * P * trans(H) + R)
                x_estimada = x + K * (z - H * x)
                P_estimada = - K * H * P;
            except np.linalg.linalg.LinAlgError:
                pass

        	# Comprobar si hemos llegado al checkpoint
            #if linea[11] == checkpoint:
            if False:
                # Si hemos llegado calculamos el error cuadrático medio
                error.append(x[0] - checkpoint_loc[it_checkpoint])
                error.append(x[1] - checkpoint_loc[it_checkpoint + 1])
                it_checkpoint += 2
                checkpoint += 1
                
        # Una vez iteramos todo el fichero hallamos el error cuadrático medio
        #DEBUG, borrar luego
        suma_error_y = 0
        suma_error_x = 0
        for k in range(0, len(error), 2):
            suma_error_x += error[k] ** 2
            suma_error_y += error[k+1] ** 2

        error.append(1)
        error_cuadratico_x = suma_error_x / len(error)
        error_cuadratico_y = suma_error_y / len(error)

        # Ponemos a 0 para la siguiente iteración
        suma_error_y = 0.0
        suma_error_x = 0.0
        error = []

        # Comprobamos si es mejor que el mejor resultado anterior
        if ((suma_error_x + suma_error_y) / 2) < error_cuadratico_minimo:
            error_cuadratico_minimo = (suma_error_x + suma_error_y) / 2

            H_best = mat.copy(H)
            R_best = mat.copy(R)
            Q_best = mat.copy(Q)

    print(time.time() - start_time)
    # Final de la ejecución
    print("Los resultados obtenidos son:")
    print(" Mínimo error cuadrático medio: " + str(error_cuadratico_minimo))
    print(" ----------------------------H----------------------------")
    print(H_best)
    print(" ----------------------------R----------------------------")
    print(R_best)
    print(" ----------------------------Q----------------------------")
    print(Q_best)

