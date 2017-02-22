#include <NewPing.h>

#define trigPin 3
#define echoPin 5
#define maxDistance 500

// Matrices de Kalman
float P = 1.0;
float P_ant = 1.0;
float P_estimada = 1.0;
float Q = 1.0;
float R = 1.0; // Cuanto mayor es menos confía en los sensores
float K = 1.0;
float H = 1.0;
float F = 1.0;

// Vectores de Kalman
float x = 1.0;
float x_ant = 1.0;
float x_estimada = 1.0;
float z = 0.0;

NewPing sonar(trigPin, echoPin, maxDistance);

void setup() {
  Serial.begin(9600);
}

void loop() {
  z = sonar.ping_cm();

  x = F * x_ant; // Hacemos una predicción sobre el estado
  P = P_ant;

  K = P / (P + R); // Calculamos la "Ganancia de Kalman"
  x_estimada = x + K * ( z - x); // Corregimos nuestra predicción en base a los sensores
  P_estimada = (1 - K) * P;

  // Visualizar en el Serial Plotter
  Serial.print(x_estimada); // Azul
  Serial.print(",");
  Serial.println(z); // Amarillo

  x_ant = x_estimada;
  P_ant = P_estimada;
}
