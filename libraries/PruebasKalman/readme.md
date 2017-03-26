#Librería de pruebas

### Forma de usarla

1. Incluir la librería mediante ```#include <PruebasKalman.h>```
2. Inicializar el objeto mediante ```PruebasKalman prueba(tam);```
3. Tras darle los valores por defecto a las matrices llamar a ```prueba.pruebaN((float*) R, (float*) Q);``` siendo N el número de la prueba


### Pruebas implementadas

* Respecto a la matriz R:
    - Pruebas 1-6: Valor muy pequeño (0.05) en el elemento Rnn, siendo n un número del 0 al 5.
    - Pruebas 7, 8 y 9: Valor muy pequeño (0.05) en las componentes de la posición, velocidad y aceleración respectivamente.
    - Pruebas 10-15: Valor muy grande (200) en el elemento Rnn, siendo n un número del 0 al 5.
    - Pruebas 16, 17 y 18: Valor muy grande (200) en las componentes de la posición, velocidad y aceleración respectivamente.

* Respecto a la matriz Q:
    - Pruebas 19-24: Valor muy pequeño (0.05) en el elemento Qnn, siendo n un número del 0 al 5.
    - Pruebas 25, 26 y 27: Valor muy pequeño (0.05) en las componentes de la posición, velocidad y aceleración respectivamente.
    - Pruebas 28-33: Valor muy grande (200) en el elemento Qnn, siendo n un número del 0 al 5.
    - Pruebas 34, 35 y 136: Valor muy grande (200) en las componentes de la posición, velocidad y aceleración respectivamente.
