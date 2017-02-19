#include <NewPing.h>

#define trigPin 3
#define echoPin 5
#define maxDistance 500

int distancia = 0;

NewPing sonar(trigPin, echoPin, maxDistance);

float P = 1.0;
float P_ant = 1.0;
float P_estimada = 1.0;
float Q = 1.0;
float R = 1.0;
float K = 1.0;
float H = 1.0;
float F = 1.0;

float x = 1.0;
float x_ant = 1.0;
float x_estimada = 1.0;

float z = 0.0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  z = sonar.ping_cm();

  x = F * x_ant;
  P = P_ant;
  
  K = P / (P + R);
  x_estimada = x + K * ( z - x);
  P_estimada = (1 - K) * P;

  // Visualizar en el Serial Plotter
  Serial.println(x_extimada);

  x_ant = x_estimada;
  P_ant = P_estimada;
}
