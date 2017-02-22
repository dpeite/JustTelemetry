# Librería para realizar las operaciones con matrices

## Descripción
 Esta librería nos permitirá realizar las operaciones entre matrices necesarias para aplicar el filtro de Kalman. Está pensada para trabajar con **matrices 3x3**, pero si necesitasemos trabajar con **matrices 2x2 sería posible dejando a 0 la última fila y columna**. Funciona así para poder ahorrarnos los bucles for (y por tanto ciclos de reloj). En un futuro se modificará para ajustarla a las necesidades finales.

## Funciones incluidas
* void mulMatrizVector(float* matriz, float* vector, float* res);
* void mulMatrizMatriz(float* matriz_izq, float* matriz_der, float* res);
* void sumaMatrizMatriz(float* matriz_izq, float* matriz_der, float* res);
* void trasponerMatriz(float* matriz, float* res);
* int invertirMatriz(float* matriz);
* void imprimirMatriz(float* matriz);
* void imprimirVector(float* vector);

## Formato esperado
### Declarar las matrices/vectores
 Las matrices se declararán de la forma **float nombre[filas][columnas]** y los vectores con la forma **float nombre[filas]**.

### Pasar como parámetro a una función
 Las matrices y vectores se pasarán a las funciones con el formato **(float*)nombre**.

## Ejemplo

```c++
    int tam = 3;
    Matrices oper(tam);
    
    float A[tam][tam];
    float B[tam][tam];
    float matriz_resultado[tam][tam];
    float v[tam];
    float vector_resultado[tam];
    
    oper.mulMatrizVector((float*)A, (float*)v, (float*)vector_resultado);
    oper.mulMatrizMatriz((float*)A, (float*)B, (float*)matriz_resultado);
    oper.sumaMatrizMatriz((float*)A, (float*)B, (float*)matriz_resultado);
    oper.trasponerMatriz((float*)A, (float*)matriz_resultado);
    oper.invertirMatriz((float*)A);
    oper.imprimirMatriz((float*)A);
    oper.imprimirVector((float*)v);
```
