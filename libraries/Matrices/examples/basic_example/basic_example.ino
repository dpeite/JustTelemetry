#include <Matrices.h>

#define n 3 // Tamaño de la matriz, solo soporta 3x3

// Matrices de ejemplo
float A[n][n];
float B[n][n];
float resultado[n][n];

// Vectores
float v[n];
float v_resultado[n];

// Creamos la instancia de la librería
Matrices oper(n);

void setup() {
  // Inicializamos el serial para poder imprimir por pantalla
  Serial.begin(9600);

  // Inicializamos las matrices
  A[0][0] = 1;
  A[0][1] = 2;
  A[0][2] = 2;

  A[1][0] = 3;
  A[1][1] = 3;
  A[1][2] = 1;

  A[2][0] = 4;
  A[2][1] = 1;
  A[2][2] = 0;

  B[0][0] = 2;
  B[1][0] = 1;
  B[2][0] = 3;

  B[0][1] = 2;
  B[1][1] = 1;
  B[2][1] = 3;

  B[0][2] = 2;
  B[1][2] = 1;
  B[2][2] = 3;

  // Inicializamos el vector
  v[1] = 2;
  v[2] = 1;

}

void loop() {

  // Operaciones disponibles
  oper.mulMatrizVector((float*)A, (float*)v, (float*)v_resultado);
  Serial.println("Multiplicación entre A y v:");
  oper.imprimirVector((float*)v_resultado);

  oper.mulMatrizMatriz((float*)A, (float*)B, (float*)resultado);
  Serial.println("Multiplicación entre A y B:");
  oper.imprimirMatriz((float*)resultado);

  oper.sumaMatrizMatriz((float*)A, (float*)B, (float*)resultado);
  Serial.println("Suma entre A y B:");
  oper.imprimirMatriz((float*)resultado);

  oper.trasponerMatriz((float*)A, (float*)resultado);
  Serial.println("Traspuesta de A:");
  oper.imprimirMatriz((float*)resultado);

  oper.invertirMatriz((float*)A);
  Serial.println("Inversa de A:");
  oper.imprimirMatriz((float*)A);

  delay(5000);
}

