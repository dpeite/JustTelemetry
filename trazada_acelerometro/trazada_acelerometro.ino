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
float x[n], x_estimada[n], z[n];

// Matrices auxiliares
float Ftras[n][n], FPant[n][n], FPantFtras[n][n];
float HP[n][n], Htras[n][n], HPHtras[n][n], HPHtras_R[n][n], PHtras[n][n];
float KHP[n][n];

// Vectores auxiliares
float Hx[n], resta_zHx[n], K_zHx[n];

float t_inicial, t_final, delta_t;

float vx, vy;
float vx_kalman, vy_kalman;
float posx, posy;
float posx_kalman, posy_kalman;

// Constantes
const float gravedad = 9.80665; // m/s

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
    t_inicial = millis();

    myIMU.readAccelData(myIMU.accelCount);  // Read the x/y/z adc values

    // Obtenemos la velocidad antes de actualizar la aceleración
    vx = z[0] * delta_t;
    vy = z[1] * delta_t;

    vx_kalman = x_estimada[0] * delta_t;
    vy_kalman = x_estimada[1] * delta_t;

    // Leemos la medición del acelerometro
    z[0] = (float)myIMU.accelCount[0] * myIMU.aRes * gravedad; // - accelBias[0]; Aceleración X
    z[1] = (float)myIMU.accelCount[1] * myIMU.aRes * gravedad; // - accelBias[1]; Aceleración Y

    // Calculamos Kalman
    // x = F * x_ant
    x[0] = x_estimada[0];
    x[1] = x_estimada[1];

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

    // x' = x + K (z - H * x)
    oper.mulMatrizVector((float*)H, (float*)x, (float*)Hx);
    oper.restaVectorVector((float*)z, (float*)Hx, (float*)resta_zHx);
    oper.mulMatrizVector((float*)K, (float*)resta_zHx, (float*)K_zHx);
    oper.sumaVectorVector((float*)x, (float*)K_zHx, (float*)x_estimada);

    // P' = P - K * H * P;
    oper.mulMatrizVector((float*)K, (float*)HP, (float*)KHP);
    oper.restaMatrizMatriz((float*)P, (float*)KHP, (float*)P_estimada);

    // Pasamos la aceleracion (x_estimada) a posición
    posx_kalman += x_estimada[0] * 0.5 * delta_t*delta_t + vx * delta_t; //px
    posy_kalman += x_estimada[1] * 0.5 * delta_t*delta_t + vy * delta_t; //px


    // Calculamos la posicion usando sólo las medidas en bruto
    posx += z[0] * 0.5 * delta_t*delta_t + vx * delta_t; //px
    posy += z[1] * 0.5 * delta_t*delta_t + vy * delta_t; //px


    t_final = millis();
    delta_t = (t_final - t_inicial) / 1000.0f;

    imprimir(posx, posy, posx_kalman, posy_kalman, delta_t);
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
