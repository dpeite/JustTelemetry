#include <Matrices.h>
#include <MPU9250.h>

// Dimensión del tamaño del estado
#define n 6

// Matrices de Kalman
float P[n][n] = {{0.1, 0, 0, 0, 0, 0}, {0, 0.1, 0, 0, 0, 0}, {0, 0, 0.1, 0, 0, 0}, {0, 0, 0, 0.1, 0, 0}, {0, 0, 0, 0, 0.1, 0}, {0, 0, 0, 0, 0, 0.1}}; // Cuanto menor más eficaz
float P_estimada[n][n] = {{0.1, 0, 0, 0, 0, 0}, {0, 0.1, 0, 0, 0, 0}, {0, 0, 0.1, 0, 0, 0}, {0, 0, 0, 0.1, 0, 0}, {0, 0, 0, 0, 0.1, 0}, {0, 0, 0, 0, 0, 0.1}};
float Q[n][n] = {{1, 0, 0, 0, 0, 0}, {0, 1, 0, 0, 0, 0}, {0, 0, 1, 0, 0, 0}, {0, 0, 0, 1, 0, 0}, {0, 0, 0, 0, 1, 0}, {0, 0, 0, 0, 0, 1}};
float R[n][n] = {{2, 0, 0, 0, 0, 0}, {0, 2, 0, 0, 0, 0}, {0, 0, 2, 0, 0, 0}, {0, 0, 0, 2, 0, 0}, {0, 0, 0, 0, 2, 0}, {0, 0, 0, 0, 0, 2}}; // Cuanto mayor menor confianza
float K[n][n] = {{1, 0, 0, 0, 0, 0}, {0, 1, 0, 0, 0, 0}, {0, 0, 1, 0, 0, 0}, {0, 0, 0, 1, 0, 0}, {0, 0, 0, 0, 1, 0}, {0, 0, 0, 0, 0, 1}};
float H[n][n] = {{1, 0, 0, 0, 0, 0}, {0, 1, 0, 0, 0, 0}, {0, 0, 1, 0, 0, 0}, {0, 0, 0, 1, 0, 0}, {0, 0, 0, 0, 1, 0}, {0, 0, 0, 0, 0, 1}};
float F[n][n] = {{1, 0, 0, 0, 0, 0}, {0, 1, 0, 0, 0, 0}, {0, 0, 1, 0, 0, 0}, {0, 0, 0, 1, 0, 0}, {0, 0, 0, 0, 1, 0}, {0, 0, 0, 0, 0, 1}};

// Vectores de Kalman
float x[n], xEstimada[n], z[n];

// Matrices auxiliares
float Ftras[n][n], FPant[n][n], FPantFtras[n][n];
float HP[n][n], Htras[n][n], HPHtras[n][n], HPHtras_R[n][n], PHtras[n][n];
float KHP[n][n];

// Vectores auxiliares
float Hx[n], resta_zHx[n], K_zHx[n];

float tiempoInicial, tiempoFinal, deltaTiempo;

float orientacion;
float velocidadX, velocidadY;
float aceleracionX, aceleracionY;
float posx, posy;
float posicionAnteriorX, posicionAnteriorY;

// Constantes
const float gravedad = 9.80665; // m/s
const float gradoToRadian = 0.0174533;

// Creamos la instancia de la librería
Matrices oper(n);
MPU9250 myIMU;

void setup() {

  Wire.begin();
  Serial.begin(38400); // Para imprimir por pantalla

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

    myIMU.getAres();  // Obtenemos la resolución del acelerómetro
  } // cierre if (c == 0x71)
  else
  {
    Serial.println("Acelerómetro no conectado");
    while (1) ; // Loop forever if communication doesn't happen
  }

  Serial.println('[');
} // Cierre setup

void loop() {

  // Comprobamos que haya nuevos datos
  if (myIMU.readByte(MPU9250_ADDRESS, INT_STATUS) & 0x01)
  {
    tiempoInicial = millis();

    myIMU.readAccelData(myIMU.accelCount);  // Read the x/y/z adc values

    // Leemos la medición del acelerometro
    aceleracionX = (float)myIMU.accelCount[0] * myIMU.aRes * gravedad; // - accelBias[0]; Aceleración X
    aceleracionY = (float)myIMU.accelCount[1] * myIMU.aRes * gravedad; // - accelBias[1]; Aceleración Y

    // Calculamos Kalman
    // x = F * x_ant
    x[0] = xEstimada[0] + xEstimada[2] * 0.5 * deltaTiempo * deltaTiempo + xEstimada[4] * deltaTiempo; // px
    x[1] = xEstimada[1] + xEstimada[3] * 0.5 * deltaTiempo * deltaTiempo + xEstimada[5] * deltaTiempo; // py
    x[2] = xEstimada[2]; // ax
    x[3] = xEstimada[3]; // ay
    x[4] = xEstimada[2] * deltaTiempo + xEstimada[4]; // vx
    x[5] = xEstimada[3] * deltaTiempo + xEstimada[5]; // vy

    orientacion = orientacionAcelerometro(x[0], x[1], posicionAnteriorX, posicionAnteriorY);

    // Obtenemos la velocidad antes de actualizar la aceleración
    velocidadX = z[2] * deltaTiempo;
    velocidadY = z[3] * deltaTiempo;

    z[2] = aceleracionX * cos((90 - orientacion) * gradoToRadian) + aceleracionY * sin((90 - orientacion) * gradoToRadian);
    z[3] = aceleracionY * cos((90 - orientacion) * gradoToRadian) - aceleracionX * sin((90 - orientacion) * gradoToRadian);

    // P = F * P_ant * F_tras + Q
    oper.mulMatrizMatriz((float*)F, (float*)P_estimada, (float*)FPant);
    oper.trasponerMatriz((float*)F, (float*)Ftras);
    oper.mulMatrizMatriz((float*)FPant, (float*)Ftras, (float*)FPantFtras);
    oper.sumaMatrizMatriz((float*)FPantFtras, (float*)Q, (float*)P);

    // K = P * Ht (H * P * Ht + R)^-1
    oper.mulMatrizMatriz((float*)H, (float*)P, (float*)HP);
    oper.trasponerMatriz((float*)H, (float*)Htras);
    oper.mulMatrizMatriz((float*)HP, (float*)Htras, (float*)HPHtras);
    oper.sumaMatrizMatriz((float*)HPHtras, (float*)R, (float*)HPHtras_R);
    oper.invertirMatriz((float*)HPHtras_R);
    oper.mulMatrizMatriz((float*)P, (float*)Htras, (float*)PHtras);
    oper.mulMatrizMatriz((float*)PHtras, (float*)HPHtras_R, (float*)K);

    // Actualizamos las coordenadas anteriores en nuestro formato
    posicionAnteriorX = xEstimada[0];
    posicionAnteriorY = xEstimada[1];

    // x' = x + K (z - H * x)
    oper.mulMatrizVector((float*)H, (float*)x, (float*)Hx);
    oper.restaVectorVector((float*)z, (float*)Hx, (float*)resta_zHx);
    oper.mulMatrizVector((float*)K, (float*)resta_zHx, (float*)K_zHx);
    oper.sumaVectorVector((float*)x, (float*)K_zHx, (float*)xEstimada);

    // P' = P - K * H * P;
    oper.mulMatrizVector((float*)K, (float*)HP, (float*)KHP);
    oper.restaMatrizMatriz((float*)P, (float*)KHP, (float*)P_estimada);

    // Calculamos la posicion usando sólo las medidas en bruto
    posx += z[2] * 0.5 * deltaTiempo * deltaTiempo + velocidadX * deltaTiempo; //px
    posy += z[3] * 0.5 * deltaTiempo * deltaTiempo + velocidadY * deltaTiempo; //px

    tiempoFinal = millis();
    deltaTiempo = (tiempoFinal - tiempoInicial) / 1000.0f;

    imprimir(posx, posy, xEstimada[0], xEstimada[1], deltaTiempo);
  }
} // Cierre Loop

/*
   Método para imprimir las coordenadas en formato LAT,LON,MILLIS
*/
void imprimir(float posx, float posy, float posx_kalman, float posy_kalman, float t) {
  Serial.print("[");
  Serial.print(posx, 8);
  Serial.print(',');
  Serial.print(posy, 8);
  Serial.print(',');
  Serial.print(posx_kalman, 8);
  Serial.print(',');
  Serial.print(posy_kalman, 8);
  Serial.print(',');
  Serial.print(t);
  Serial.println("],");
}

float orientacionAcelerometro(float posX, float posY, float posAnteriorX, float posAnteriorY) {
  return 90.0 * gradoToRadian - (2 * M_PI + atan2(posY - posAnteriorY, posX - posAnteriorX));
}
