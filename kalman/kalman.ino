
#include <Matrices.h>
#include <TinyGPS.h>
#include <MPU9250.h>
#include <math.h>

// Dimensión del tamaño del estado
#define n 6

// Matrices de Kalman
float P[n][n] = {{0.1, 0, 0, 0, 0, 0}, {0, 0.1, 0, 0, 0, 0}, {0, 0, 0.1, 0, 0, 0}, {0, 0, 0, 0.1, 0, 0}, {0, 0, 0, 0, 0.1, 0}, {0, 0, 0, 0, 0, 0.1}}; // Cuanto menor más eficaz
float PEstimada[n][n] = {{0.1, 0, 0, 0, 0, 0}, {0, 0.1, 0, 0, 0, 0}, {0, 0, 0.1, 0, 0, 0}, {0, 0, 0, 0.1, 0, 0}, {0, 0, 0, 0, 0.1, 0}, {0, 0, 0, 0, 0, 0.1}};
float Q[n][n] = {{1, 0, 0, 0, 0, 0}, {0, 1, 0, 0, 0, 0}, {0, 0, 1, 0, 0, 0}, {0, 0, 0, 1, 0, 0}, {0, 0, 0, 0, 1, 0}, {0, 0, 0, 0, 0, 1}};
float R[n][n] = {{2, 0, 0, 0, 0, 0}, {0, 2, 0, 0, 0, 0}, {0, 0, 2, 0, 0, 0}, {0, 0, 0, 2, 0, 0}, {0, 0, 0, 0, 2, 0}, {0, 0, 0, 0, 0, 2}}; // Cuanto mayor menor confianza
float K[n][n] = {{1, 0, 0, 0, 0, 0}, {0, 1, 0, 0, 0, 0}, {0, 0, 1, 0, 0, 0}, {0, 0, 0, 1, 0, 0}, {0, 0, 0, 0, 1, 0}, {0, 0, 0, 0, 0, 1}};
float H[n][n] = {{1, 0, 0, 0, 0, 0}, {0, 1, 0, 0, 0, 0}, {0, 0, 1, 0, 0, 0}, {0, 0, 0, 1, 0, 0}, {0, 0, 0, 0, 1, 0}, {0, 0, 0, 0, 0, 1}};
float F[n][n] = {{1, 0, 0, 0, 0, 0}, {0, 1, 0, 0, 0, 0}, {0, 0, 1, 0, 0, 0}, {0, 0, 0, 1, 0, 0}, {0, 0, 0, 0, 1, 0}, {0, 0, 0, 0, 0, 1}};

// Vectores de Kalman
float x[n], xEstimada[n], z[n];

// Matrices auxiliares
float Ftras[n][n], FPant[n][n], FPantFtras[n][n];
float HP[n][n], Htras[n][n], HPHtras[n][n], HPHtrasR[n][n], PHtras[n][n];
float KHP[n][n];

// Vectores auxiliares
float Hx[n], restaZHx[n], KZHx[n];

// Variables del tiempo
long tiempoInicial, tiempoFinal;
float deltaTiempo;
unsigned long tiempoMuestra;

// Variables del GPS
float latitud, longitud; // Posición actual
float latitudInicial, longitudInicial; // Posición inicial
float latitudAnterior, longitudAnterior; // Posición anterior
float velocidad;
float orientacion; // Respecto al norte
float angulo; // Entre la posición inicial y la actual
unsigned long distanciaOrigen, distanciaRecorrida;
int contadorMismaPosicion; // Si estamos en la misma posición incrementa su valor

// Variables del acelerómetro
float aceleracionX, aceleracionY;
float posicionAnteriorX, posicionAnteriorY;

// Constantes
const float grado_to_radian = 0.0174533; // 1 grado son 0.0174533
const float gravedad = 9.80665; // m/s
const float km_to_ms = 0.277778;
const int perdidaGPS = 3; // Indica a partir de cuando consideramos que estamos en pérdida

// Creamos la instancia de la librería
Matrices oper(n);
MPU9250 myIMU;
TinyGPS gps;

static void smartDelay(unsigned long ms);

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
    smartDelay(500);
    gps.f_get_position(&latitud, &longitud, &tiempoMuestra);

    if (latitud != TinyGPS::GPS_INVALID_F_ANGLE) {

      // Para comprobar que ha detectado bien la posición esperamos hasta recibir 5 veces las mismas coordenadas
      if (latitud == latitudInicial && longitud == longitudInicial) {
        cont++;

        if (cont == 5) {
          Serial.println("Posición fijada");
          break;
        }
      } else {
        // Si no coinciden tomamos las que acabamos de coger como iniciales
        latitudInicial = latitud;
        longitudInicial = longitud;
      }
    } // Cierre if valid coords
  } // Cierre while

  latitudAnterior = latitudInicial;
  longitudAnterior = longitudInicial;

  Serial.println('[');

} // Cierre setup

void loop() {

  // Comprobamos que haya nuevos datos
  if (myIMU.readByte(MPU9250_ADDRESS, INT_STATUS) & 0x01)
  {
    tiempoInicial = millis();

    smartDelay(50);
    gps.f_get_position(&latitud, &longitud, &tiempoMuestra);

    distanciaOrigen = (unsigned long) gps.distance_between(latitudInicial, longitudInicial, latitud, longitud);
    distanciaRecorrida += (unsigned long) gps.distance_between(latitudAnterior, longitudAnterior, latitud, longitud);

    if (!((latitudAnterior == latitud) && (longitudAnterior == longitud))) {
      orientacion = gps.course_to(latitudAnterior, longitudAnterior, latitud, longitud);
    }

    angulo = gps.course_to(latitudInicial, longitudInicial, latitud, longitud);
    velocidad = gps.f_speed_kmph() * km_to_ms;

    // La resolución la he movido arriba, si da error volver a ponerlas aquí
    myIMU.readAccelData(myIMU.accelCount);  // Read the x/y/z adc values

    aceleracionX = (float)myIMU.accelCount[0] * myIMU.aRes * gravedad; // - accelBias[0]; Aceleración X
    aceleracionY = (float)myIMU.accelCount[1] * myIMU.aRes * gravedad; // - accelBias[1]; Aceleración Y

    // Comprobamos que nos hemos movido del origen
    if (distanciaOrigen != 0) {
      z[0] = distanciaOrigen * sin(grado_to_radian * angulo); // Para invertir la imagen
      z[1] = distanciaOrigen * cos(grado_to_radian * angulo);
      z[2] = aceleracionX * cos((90 - orientacion) * grado_to_radian) + aceleracionY * sin((90 - orientacion) * grado_to_radian);
      z[3] = aceleracionY * cos((90 - orientacion) * grado_to_radian) - aceleracionX * sin((90 - orientacion) * grado_to_radian);
      z[4] = velocidad * sin(grado_to_radian * orientacion);
      z[5] = velocidad * cos(grado_to_radian * orientacion);
    } else {
      //z[0] = 0.0;
      //z[1] = 0.0;
      z[2] = 0.0;
      z[3] = 0.0;
      z[4] = 0.0;
      z[5] = 0.0;
    }

    // Calculamos Kalman
    // x = F * x_ant
    x[0] = xEstimada[0] + xEstimada[2] * 0.5 * deltaTiempo*deltaTiempo + xEstimada[4] * deltaTiempo; // px
    x[1] = xEstimada[1] + xEstimada[3] * 0.5 * deltaTiempo*deltaTiempo + xEstimada[5] * deltaTiempo; // py
    x[2] = xEstimada[2]; // ax
    x[3] = xEstimada[3]; // ay
    x[4] = xEstimada[2] * deltaTiempo + xEstimada[4]; // vx
    x[5] = xEstimada[3] * deltaTiempo + xEstimada[5]; // vy

    // Detección de pérdida del GPS
    if ((latitudAnterior == latitud) && (longitudAnterior == longitud)) {
      contadorMismaPosicion++;
      if (contadorMismaPosicion >= perdidaGPS) {

        orientacion = orientacionAcelerometro(x[0], x[1], posicionAnteriorX, posicionAnteriorY);
        z[0] = x[0]; //px
        z[1] = x[1];
        z[4] = x[4]; // vx
        z[5] = x[5];
      }
    } else {
      contadorMismaPosicion = 0;
    }

    // P = F * P_ant * F_tras + Q
    oper.mulMatrizMatriz((float*)F, (float*)PEstimada, (float*)FPant);
    oper.trasponerMatriz((float*)F, (float*)Ftras);
    oper.mulMatrizMatriz((float*)FPant, (float*)Ftras, (float*)FPantFtras);
    oper.sumaMatrizMatriz((float*)FPantFtras, (float*)Q, (float*)P);

    // K = P * Ht (H * P * Ht + R)^-1
    oper.mulMatrizMatriz((float*)H, (float*)P, (float*)HP);
    oper.trasponerMatriz((float*)H, (float*)Htras);
    oper.mulMatrizMatriz((float*)HP, (float*)Htras, (float*)HPHtras);
    oper.sumaMatrizMatriz((float*)HPHtras, (float*)R, (float*)HPHtrasR);
    oper.invertirMatriz((float*)HPHtrasR);
    oper.mulMatrizMatriz((float*)P, (float*)Htras, (float*)PHtras);
    oper.mulMatrizMatriz((float*)PHtras, (float*)HPHtrasR, (float*)K);

    // Actualizamos las coordenadas anteriores en nuestro formato
    posicionAnteriorX = xEstimada[0];
    posicionAnteriorY = xEstimada[1];

    // x' = x + K (z - H * x)
    oper.mulMatrizVector((float*)H, (float*)x, (float*)Hx);
    oper.restaVectorVector((float*)z, (float*)Hx, (float*)restaZHx);
    oper.mulMatrizVector((float*)K, (float*)restaZHx, (float*)KZHx);
    oper.sumaVectorVector((float*)x, (float*)KZHx, (float*)xEstimada);

    // P' = P - K * H * P;
    oper.mulMatrizVector((float*)K, (float*)HP, (float*)KHP);
    oper.restaMatrizMatriz((float*)P, (float*)KHP, (float*)PEstimada);

    imprimir(xEstimada[0], xEstimada[1]);

    tiempoFinal = millis();
    deltaTiempo = (tiempoFinal - tiempoInicial) / 1000.0f;

    // Actualizamos las coordenadas anteriores en formato coordenadas estándar
    latitudAnterior = latitud;
    longitudAnterior = longitud;
  }
} // Cierre Loop

/*
   No tengo muy claro como se supone que va este método, venía en el ejemplo de la librería.
   Parece que el parámetro que se le pasa es el tiempo entre muestras
*/
static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do
  {
    while (Serial1.available())
      gps.encode(Serial1.read());
  } while (millis() - start < ms);
}

float orientacionAcelerometro(float lat, float lon, float lat_ant, float lon_ant) {
  return 90.0 * grado_to_radian - (2 * M_PI + atan2(lon - lon_ant, lat - lat_ant));
}

/*
   Método para imprimir las coordenadas en formato [LAT_KAL,LON_KAL,LAT_ORI,LON_ORI,MILLIS]
*/
void imprimir(float lat_kal, float lon_kal) {
  Serial.print('[');
  Serial.print(lat_kal, 8);
  Serial.print(',');
  Serial.print(lon_kal, 8);
  Serial.print(',');
  Serial.print(z[0], 8);
  Serial.print(',');
  Serial.print(z[1], 8);
  Serial.print(',');
  Serial.print(millis());
  Serial.println("],");
}

