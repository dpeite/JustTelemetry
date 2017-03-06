/*Hardware setup:
  MPU9250 Breakout --------- Arduino
  VDD ---------------------- 3.3V
  VDDI --------------------- 3.3V
  SDA ----------------------- A4
  SCL ----------------------- A5
  GND ---------------------- GND
*/

#include <Matrices.h>
#include "MPU9250.h"

// Dimensión del estado
#define n 6
#define lim 300

int cont = 0;
float or_inicial = 0.0;
float hist[5];

// Matrices de Kalman
float P[n][n] = {{0.1, 0, 0, 0, 0, 0}, {0, 0.1, 0, 0, 0, 0}, {0, 0, 0.1, 0, 0, 0}, {0, 0, 0, 0.1, 0, 0}, {0, 0, 0, 0, 0.1, 0}, {0, 0, 0, 0, 0, 0.1}}; // Cuanto menor más eficaz
float P_ant[n][n] = {{0.1, 0, 0, 0, 0, 0}, {0, 0.1, 0, 0, 0, 0}, {0, 0, 0.1, 0, 0, 0}, {0, 0, 0, 0.1, 0, 0}, {0, 0, 0, 0, 0.1, 0}, {0, 0, 0, 0, 0, 0.1}}; // P_ant no hace falta, usamos P_estimada
float P_estimada[n][n] = {{0.1, 0, 0, 0, 0, 0}, {0, 0.1, 0, 0, 0, 0}, {0, 0, 0.1, 0, 0, 0}, {0, 0, 0, 0.1, 0, 0}, {0, 0, 0, 0, 0.1, 0}, {0, 0, 0, 0, 0, 0.1}};
float Q[n][n] = {{1, 0, 0, 0, 0, 0}, {0, 1, 0, 0, 0, 0}, {0, 0, 1, 0, 0, 0}, {0, 0, 0, 1, 0, 0}, {0, 0, 0, 0, 1, 0}, {0, 0, 0, 0, 0, 1}};
float R[n][n] = {{2, 0, 0, 0, 0, 0}, {0, 2, 0, 0, 0, 0}, {0, 0, 2, 0, 0, 0}, {0, 0, 0, 2, 0, 0}, {0, 0, 0, 0, 2, 0}, {0, 0, 0, 0, 0, 2}}; // Cuanto mayor menor confianza
float K[n][n] = {{1, 0, 0, 0, 0, 0}, {0, 1, 0, 0, 0, 0}, {0, 0, 1, 0, 0, 0}, {0, 0, 0, 1, 0, 0}, {0, 0, 0, 0, 1, 0}, {0, 0, 0, 0, 0, 1}};
float H[n][n] = {{1, 0, 0, 0, 0, 0}, {0, 1, 0, 0, 0, 0}, {0, 0, 1, 0, 0, 0}, {0, 0, 0, 1, 0, 0}, {0, 0, 0, 0, 1, 0}, {0, 0, 0, 0, 0, 1}};
float F[n][n] = {{1, 0, 0, 0, 0, 0}, {0, 1, 0, 0, 0, 0}, {0, 0, 1, 0, 0, 0}, {0, 0, 0, 1, 0, 0}, {0, 0, 0, 0, 1, 0}, {0, 0, 0, 0, 0, 1}};

// Vectores de Kalman
float x[n];
float x_ant[n]; // x_ant no hace falta, usamos x_estimada
float x_estimada[n];

float z[n];

// Matrices auxiliares
float Ftras[n][n];
float FPant[n][n];
float FPantFtras[n][n];

float HP[n][n];
float Htras[n][n];
float HPHtras[n][n];
float HPHtras_R[n][n];
float PHtras[n][n];

float KHP[n][n];

// Vectores auxiliares
float Hx[n];
float resta_zHx[n];
float K_zHx[n];

// Para calcular la orientación
long t_inicial, t_final;
double delta_t, orientacion;

// Creamos la instancia de la librería
Matrices oper(n);
MPU9250 myIMU;

void setup() {

  Wire.begin();
  Serial.begin(38400);

  // Read the WHO_AM_I register, this is a good test of communication
  byte c = myIMU.readByte(MPU9250_ADDRESS, WHO_AM_I_MPU9250);

  if (c == 0x71) // WHO_AM_I should always be 0x68
  {
    // Start by performing self test
    myIMU.MPU9250SelfTest(myIMU.SelfTest);

    // Calibrate gyro and accelerometers, load biases in bias registers
    myIMU.calibrateMPU9250(myIMU.gyroBias, myIMU.accelBias);
    myIMU.initMPU9250();

    // Get magnetometer calibration from AK8963 ROM
    myIMU.initAK8963(myIMU.magCalibration);
  } // cierre if (c == 0x71)
  else
  {
    Serial.println("Acelerómetro no conectado");
    while (1) ; // Loop forever if communication doesn't happen
  }

  pinMode(13, OUTPUT);

} // Cierre setup

void loop() {

  // Comprobamos que haya nuevos datos
  if (myIMU.readByte(MPU9250_ADDRESS, INT_STATUS) & 0x01)
  {

    myIMU.readGyroData(myIMU.gyroCount);  // Read the x/y/z adc values
    myIMU.getGres();

    t_inicial = millis();

    // Calculate the gyro value into actual degrees per second
    // This depends on scale being set
    z[0] = (float)myIMU.gyroCount[0] * myIMU.gRes; // En grados segundo
    z[1] = (float)myIMU.gyroCount[1] * myIMU.gRes;
    z[2] = (float)myIMU.gyroCount[2] * myIMU.gRes; // - myIMU.gyroBias[2];

    // Calculamos Kalman
    // x = F * x_ant
    x[0] = x_ant[0] + x_ant[2]*0.5*delta_t*delta_t + x_ant[4]*delta_t;
    x[1] = x_ant[1] + x_ant[3]*0.5*delta_t*delta_t + x_ant[5]*delta_t;
    x[2] = x_ant[2];
    x[3] = x_ant[3];
    x[4] = x_ant[2]*delta_t + x_ant[4];
    x[5] = x_ant[3]*delta_t + x_ant[5];

    // P = F * P_ant * F_tras + Q
    //    oper.mulMatrizMatriz((float*)F, (float*)P_estimada, (float*)FPant);
    //    oper.trasponerMatriz((float*)F, (float*)Ftras);
    //    oper.mulMatrizMatriz((float*)FPant, (float*)Ftras, (float*)FPantFtras);
    //    oper.sumaMatrizMatriz((float*)FPantFtras, (float*)Q, (float*)P);

    // K = P * Ht (H * P * Ht + R)^-1
    oper.mulMatrizMatriz((float*)H, (float*)P, (float*)HP);
    oper.trasponerMatriz((float*)H, (float*)Htras);
    oper.mulMatrizMatriz((float*)HP, (float*)Htras, (float*)HPHtras);
    oper.sumaMatrizMatriz((float*)HPHtras, (float*)R, (float*)HPHtras_R);
    oper.invertirMatriz((float*)HPHtras_R);
    oper.mulMatrizMatriz((float*)P, (float*)Htras, (float*)PHtras);
    oper.mulMatrizMatriz((float*)PHtras, (float*)HPHtras_R, (float*)K);


    // x' = x + K (z - H * x)
    oper.mulMatrizVector((float*)H, (float*)x, (float*)Hx);
    oper.restaVectorVector((float*)z, (float*)Hx, (float*)resta_zHx);
    oper.mulMatrizVector((float*)K, (float*)resta_zHx, (float*)K_zHx);
    oper.sumaVectorVector((float*)x, (float*)K_zHx, (float*)x_estimada);

    // P' = P - K * H * P;
    oper.mulMatrizVector((float*)K, (float*)HP, (float*)KHP);
    oper.restaMatrizMatriz((float*)P, (float*)KHP, (float*)P_estimada);

    t_final = millis();
    delta_t = (t_final - t_inicial);
    
    orientacion += delta_t * x_estimada[2] / 1000.0f;

    Serial.println(orientacion);
   
    //grafica(x_estimada);
  }
} // Cierre Loop

/*
   Imprime la aceleración en el eje x (azul), eje y (naranja) y eje z (rojo)
*/
void grafica(float *z1) {
  Serial.print(z1[0]); // Azul
  Serial.print(",");
  Serial.print(z1[1]); // Naranja
  Serial.print(",");
  Serial.println(z1[2]); // Rojo
}
