/*
  Matrices.h
*/
#ifndef PruebasKalman_h
#define PruebasKalman_h

#include "Arduino.h"

class PruebasKalman
{
  public:
    PruebasKalman(int tam);
    void prueba1(float* R, float* Q);
    void prueba2(float* R, float* Q);
    void prueba3(float* R, float* Q);
    void prueba4(float* R, float* Q);
    void prueba5(float* R, float* Q);
  private:
    int _tam;
};

#endif