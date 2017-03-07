#Filtro de Kalman

 Nuestra implementación del filtro de Kalman fusiona la información proveniente del acelerómetro, giroscopio y del GPS.
 
 El vector de estados de nuestro sistema estará formado por:
```
x = [px, py, vx, vy, ax, ay]

Siendo:
px = Posición en el eje x
py = Posición en el eje y
vx = Velocidad respecto al eje x
vy = Velocidad respecto al eje y
ax = Aceleración respecto al eje x
ay = Aceleración respecto al eje y
```

Adems, necesitamos almacenar la orientación del vehículo (theta) que tomaremos respecto al norte

La matriz F, que describe la relación entre un estado y el siguiente, es la siguiente:

