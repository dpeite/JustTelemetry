/*
  Matrices.h
*/
#ifndef Matrices_h
#define Matrices_h

#include "Arduino.h"

class Matrices
{
  public:
    Matrices(int tam);
    void mulMatrizVector(float* matriz, float* vector, float* res);
    void mulMatrizMatriz(float* matriz_izq, float* matriz_der, float* res);
    void sumaMatrizMatriz(float* matriz_izq, float* matriz_der, float* res);
    void trasponerMatriz(float* matriz, float* res);
    int invertirMatriz(float* A);
    void imprimirMatriz(float* A);
    void imprimirVector(float* res);
  private:
    int _tam;
};

#endif