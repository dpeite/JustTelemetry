
/*Hardware setup:
  MPU9250 Breakout --------- Arduino
  VDD ---------------------- 3.3V
  VDDI --------------------- 3.3V
  SDA ----------------------- A4
  SCL ----------------------- A5
  GND ---------------------- GND
*/

#include <Matrices.h>
#include <TinyGPS.h>
#include <MPU9250.h>

// Desomentar para imprimir en formato JSON para la web
//#define JSON

// Descomentar para enviar las coordenadas originales junto a las obtenidas por Kalman
//#define COMPARE

// Descomentar para enviar la distancia
#define DIST

// Dimensión del tamaño del estado
#define n 6
#define RESET_ORIENTACION 5

// Matrices de Kalman
float P[n][n] = {{0.1, 0, 0, 0, 0, 0}, {0, 0.1, 0, 0, 0, 0}, {0, 0, 0.1, 0, 0, 0}, {0, 0, 0, 0.1, 0, 0}, {0, 0, 0, 0, 0.1, 0}, {0, 0, 0, 0, 0, 0.1}}; // Cuanto menor más eficaz
float P_estimada[n][n] = {{0.1, 0, 0, 0, 0, 0}, {0, 0.1, 0, 0, 0, 0}, {0, 0, 0.1, 0, 0, 0}, {0, 0, 0, 0.1, 0, 0}, {0, 0, 0, 0, 0.1, 0}, {0, 0, 0, 0, 0, 0.1}};
float Q[n][n] = {{1, 0, 0, 0, 0, 0}, {0, 1, 0, 0, 0, 0}, {0, 0, 1, 0, 0, 0}, {0, 0, 0, 1, 0, 0}, {0, 0, 0, 0, 1, 0}, {0, 0, 0, 0, 0, 1}};
float R[n][n] = {{2, 0, 0, 0, 0, 0}, {0, 2, 0, 0, 0, 0}, {0, 0, 2, 0, 0, 0}, {0, 0, 0, 2, 0, 0}, {0, 0, 0, 0, 2, 0}, {0, 0, 0, 0, 0, 2}}; // Cuanto mayor menor confianza
float K[n][n] = {{1, 0, 0, 0, 0, 0}, {0, 1, 0, 0, 0, 0}, {0, 0, 1, 0, 0, 0}, {0, 0, 0, 1, 0, 0}, {0, 0, 0, 0, 1, 0}, {0, 0, 0, 0, 0, 1}};
float H[n][n] = {{1, 0, 0, 0, 0, 0}, {0, 1, 0, 0, 0, 0}, {0, 0, 1, 0, 0, 0}, {0, 0, 0, 1, 0, 0}, {0, 0, 0, 0, 1, 0}, {0, 0, 0, 0, 0, 1}};
float F[n][n] = {{1, 0, 0, 0, 0, 0}, {0, 1, 0, 0, 0, 0}, {0, 0, 1, 0, 0, 0}, {0, 0, 0, 1, 0, 0}, {0, 0, 0, 0, 1, 0}, {0, 0, 0, 0, 0, 1}};

// Vectores de Kalman
float x[n];
float x_estimada[n];

float z[n];

// Matrices auxiliares
float Ftras[n][n], FPant[n][n], FPantFtras[n][n];

float HP[n][n], Htras[n][n], HPHtras[n][n], HPHtras_R[n][n], PHtras[n][n];

float KHP[n][n];

// Vectores auxiliares
float Hx[n];
float resta_zHx[n];
float K_zHx[n];

// Para calcular la orientación
long t_inicial, t_final;

//Variables del GPS
float flat, flon, medida_giroscopio, lat_ini, lon_ini, velocidad;
float flat_ant, flon_ant;
float orientacion, delta_t;
float angulo = 0.0;
unsigned long age, distancia, dist_acumulada;
int cont_reset_orientacion = 0;

boolean first = true;

// Constantes
const float grado_to_radian = 0.0174533;
const float gravedad = 9.80665; // m/s

// Creamos la instancia de la librería
Matrices oper(n);
MPU9250 myIMU;
TinyGPS gps;

static void smartdelay(unsigned long ms);

void setup() {

  Wire.begin();
  Serial.begin(38400); // Para imprimir por pantalla
  Serial1.begin(112500); // Para leer el GPS

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

    myIMU.getGres();
    myIMU.getAres();  // Obtenemos la resolución del acelerómetro
  } // cierre if (c == 0x71)
  else
  {
    Serial.println("Acelerómetro no conectado");
    while (1) ; // Loop forever if communication doesn't happen
  }

  Serial.println("Esperando a adquirir la posición...");
  int cont = 0;
  while (1) {
    smartdelay(500);
    gps.f_get_position(&flat, &flon, &age);

    if (flat != TinyGPS::GPS_INVALID_F_ANGLE) {

      // Para comprobar que ha detectado bien la posición esperamos hasta recibir 5 veces las mismas coordenadas
      if (flat == lat_ini && flon == lon_ini) {
        cont++;

        if (cont == 5) {
          Serial.println("Posición fijada");
          break;
        }
      } else {
        // Si no coinciden tomamos las que acabamos de coger como iniciales
        lat_ini = flat;
        lon_ini = flon;
      }
    } // Cierre if valid coords
  } // Cierre while

  flat_ant = lat_ini;
  flon_ant = lon_ini;

#if defined(JSON) || defined(COMPARE)
  Serial.println('[');
#endif
} // Cierre setup

void loop() {

  // Comprobamos que haya nuevos datos
  if (myIMU.readByte(MPU9250_ADDRESS, INT_STATUS) & 0x01)
  {
    t_inicial = millis();

    smartdelay(100);
    gps.f_get_position(&flat, &flon, &age);
    
    distancia = (unsigned long) gps.distance_between(lat_ini, lon_ini, flat, flon);
    dist_acumulada += (unsigned long) gps.distance_between(flat_ant, flon_ant, flat, flon);
    orientacion = gps.course_to(flat_ant, flon_ant, flat, flon);
    angulo = gps.course_to(lat_ini, lon_ini, flat, flon);

    // La resolución la he movido arriba, si da error volver a ponerlas aquí
    //myIMU.readGyroData(myIMU.gyroCount);  // Read the x/y/z adc values
    myIMU.readAccelData(myIMU.accelCount);  // Read the x/y/z adc values

    // Valor del giroscopio en º/s
    medida_giroscopio = (float)myIMU.gyroCount[2] * myIMU.gRes; // - myIMU.gyroBias[2];
    //orientacion += delta_t * medida_giroscopio * grado_to_radian; // He movido aquí el cálculo de la orientación
    //cont_reset_orientacion++;

    //    if (first || cont_reset_orientacion == RESET_ORIENTACION) {
    //      first = false;
    //      cont_reset_orientacion = 0;
    //      orientacion = gps.course_to(flat_ant, flon_ant, flat, flon);
    //    }

    // Comprobamos que nos hemos movido del origen
    if (distancia != 0) {
      z[0] = distancia * cos(grado_to_radian * angulo) * (-1); // Para invertir la imagen
      z[1] = distancia * sin(grado_to_radian * angulo);
      z[2] = (float)myIMU.accelCount[0] * myIMU.aRes * gravedad; // - accelBias[0]; Aceleración X
      z[3] = (float)myIMU.accelCount[1] * myIMU.aRes * gravedad; // - accelBias[1]; Aceleración Y
      z[4] = velocidad * cos(grado_to_radian * orientacion);
      z[5] = velocidad * sin(grado_to_radian * orientacion);
    } else {
      // Si no nos movemos la velocidad y la aceleración son 0. Así evitamos errores de sincronía entre GPS e IMU
      // z[2] = 0.0;
      // z[3] = 0.0;
      // z[4] = 0.0;
      // z[5] = 0.0;
    }

    // Calculamos Kalman
    // x = F * x_ant
    x[0] = x_estimada[0] + x_estimada[2] * 0.5 * delta_t*delta_t + x_estimada[4] * delta_t; //px
    x[1] = x_estimada[1] + x_estimada[3] * 0.5 * delta_t*delta_t + x_estimada[5] * delta_t; //py
    x[2] = x_estimada[2]; // ax
    x[3] = x_estimada[3]; //ay
    x[4] = x_estimada[2] * delta_t + x_estimada[4]; // vx
    x[5] = x_estimada[3] * delta_t + x_estimada[5]; // vy

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

#ifdef JSON
    imprimir_json(x_estimada[0], x_estimada[1]);
#endif

#ifdef DIST
    imprimir_json_dist(x_estimada[0], x_estimada[1], dist_acumulada);
#endif

#ifdef COMPARE
    imprimir_json_comp(x_estimada[0], x_estimada[1], z[0], z[1]);
#endif

    //#ifndef JSON
    //    imprimir_coordenadas(x_estimada[0], x_estimada[1]);
    //#endif

    t_final = millis();
    delta_t = (t_final - t_inicial) / 1000.0f;
  }
} // Cierre Loop

/*
   No tengo muy claro como se supone que va este método, venía en el ejemplo de la librería.
   Parece que el parámetro que se le pasa es el tiempo entre muestras
*/
static void smartdelay(unsigned long ms)
{
  unsigned long start = millis();
  do
  {
    while (Serial1.available())
      gps.encode(Serial1.read());
  } while (millis() - start < ms);
}

/*
   Método para imprimir las coordenadas en formato LAT,LON,MILLIS
*/
void imprimir_coordenadas_tiempo(float lat, float lon) {
  Serial.print(lat, 8);
  Serial.print(',');
  Serial.print(lon, 8);
  Serial.print(',');
  Serial.println(millis());
}

/*
   Método para imprimir las coordenadas en formato LAT,LON
*/
void imprimir_coordenadas(float lat, float lon) {
  Serial.print(lat, 8);
  Serial.print(',');
  Serial.println(lon, 8);
}

/*
   Método para imprimir las coordenadas en formato [LAT,LON,MILLIS]
*/
void imprimir_json(float lat, float lon) {
  Serial.print('[');
  Serial.print(lat, 8);
  Serial.print(',');
  Serial.print(lon, 8);
  Serial.print(',');
  Serial.print(millis());
  Serial.print(']');
  Serial.println(',');
}

/*
   Método para imprimir las coordenadas en formato [LAT, LON, DIST, MILLIS]
*/
void imprimir_json_dist(float lat, float lon, unsigned long dist) {
  Serial.print('[');
  Serial.print(lat, 8);
  Serial.print(',');
  Serial.print(lon, 8);
  Serial.print(',');
  Serial.print(dist);
  Serial.print(',');
  Serial.print(millis());
  Serial.print(']');
  Serial.println(',');
}

/*
   Método para imprimir las coordenadas en formato [LAT_KAL,LON_KAL,LAT_ORI,LON_ORI,MILLIS]
*/
void imprimir_json_comp(float lat_kal, float lon_kal, float lat_ori, float lon_ori) {
  Serial.print('[');
  Serial.print(lat_kal, 8);
  Serial.print(',');
  Serial.print(lon_kal, 8);
  Serial.print(',');
  Serial.print(lat_ori, 8);
  Serial.print(',');
  Serial.print(lon_ori, 8);
  Serial.print(',');
  Serial.print(millis());
  Serial.print(']');
  Serial.println(',');
}

