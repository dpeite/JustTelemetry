
#include <Matrices.h>
#include <TinyGPS.h>
#include <MPU9250.h>
#include <math.h>

// Dimensión del tamaño del estado
#define n 6

int cont_coincide = 0;

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
float Hx[n], resta_zHx[n], K_zHx[n];

// Para calcular la orientación
long t_inicial, t_final;
float posx_ant, posy_ant;

//Variables del GPS
float flat, flon, lat_ini, lon_ini, velocidad;
float flat_ant, flon_ant;
float orientacion, delta_t;
float angulo = 0.0; // Respecto a la posición inicial
unsigned long age, distancia, dist_acumulada, dist_actual;

float acelx, acely;

// Constantes
const float grado_to_radian = 0.0174533; // 1 grado son 0.0174533
const float gravedad = 9.80665; // m/s
const float km_to_ms = 0.277778;
const int perdida_gps = 3; // Indica a partir de cuando consideramos que estamos en pérdida

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

  Serial.println('[');

} // Cierre setup

void loop() {

  // Comprobamos que haya nuevos datos
  if (myIMU.readByte(MPU9250_ADDRESS, INT_STATUS) & 0x01)
  {
    t_inicial = millis();

    smartdelay(50);
    gps.f_get_position(&flat, &flon, &age);

    distancia = (unsigned long) gps.distance_between(lat_ini, lon_ini, flat, flon);
    dist_actual = (unsigned long) gps.distance_between(flat_ant, flon_ant, flat, flon);
    dist_acumulada += dist_actual;

    if (!((flat_ant == flat) && (flon_ant == flon))) {
      orientacion = gps.course_to(flat_ant, flon_ant, flat, flon);
    }

    angulo = gps.course_to(lat_ini, lon_ini, flat, flon);
    velocidad = gps.f_speed_kmph() * km_to_ms;

    // La resolución la he movido arriba, si da error volver a ponerlas aquí
    myIMU.readAccelData(myIMU.accelCount);  // Read the x/y/z adc values

    acelx = (float)myIMU.accelCount[0] * myIMU.aRes * gravedad; // - accelBias[0]; Aceleración X
    acely = (float)myIMU.accelCount[1] * myIMU.aRes * gravedad; // - accelBias[1]; Aceleración Y

    // Comprobamos que nos hemos movido del origen
    if (distancia != 0) {
      z[0] = distancia * sin(grado_to_radian * angulo); // Para invertir la imagen
      z[1] = distancia * cos(grado_to_radian * angulo);
      z[2] = acelx * cos((90 - orientacion) * grado_to_radian) + acely * sin((90 - orientacion) * grado_to_radian);
      z[3] = acely * cos((90 - orientacion) * grado_to_radian) - acelx * sin((90 - orientacion) * grado_to_radian);
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
    x[0] = x_estimada[0] + x_estimada[2] * 0.5 * delta_t*delta_t + x_estimada[4] * delta_t; // px
    x[1] = x_estimada[1] + x_estimada[3] * 0.5 * delta_t*delta_t + x_estimada[5] * delta_t; // py
    x[2] = x_estimada[2]; // ax
    x[3] = x_estimada[3]; // ay
    x[4] = x_estimada[2] * delta_t + x_estimada[4]; // vx
    x[5] = x_estimada[3] * delta_t + x_estimada[5]; // vy

    if ((flat_ant == flat) && (flon_ant == flon)) {
      cont_coincide++;
      if (cont_coincide >= perdida_gps) {

        orientacion = orientacion_acelerometro(x[0], x[1], posx_ant, posy_ant);
        z[0] = x[0]; //px
        z[1] = x[1];
        z[4] = x[4]; // vx
        z[5] = x[5];
      }
    } else {
      cont_coincide = 0;
    }

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

    // En nuestro formato
    posx_ant = x_estimada[0];
    posy_ant = x_estimada[1];

    // x' = x + K (z - H * x)
    oper.mulMatrizVector((float*)H, (float*)x, (float*)Hx);
    oper.restaVectorVector((float*)z, (float*)Hx, (float*)resta_zHx);
    oper.mulMatrizVector((float*)K, (float*)resta_zHx, (float*)K_zHx);
    oper.sumaVectorVector((float*)x, (float*)K_zHx, (float*)x_estimada);

    // P' = P - K * H * P;
    oper.mulMatrizVector((float*)K, (float*)HP, (float*)KHP);
    oper.restaMatrizMatriz((float*)P, (float*)KHP, (float*)P_estimada);

    imprimir_json_comp(x_estimada[0], x_estimada[1]);

    t_final = millis();
    delta_t = (t_final - t_inicial) / 1000.0f;

    // En formato coordenadas estándar
    flat_ant = flat;
    flon_ant = flon;


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

float orientacion_acelerometro(float lat, float lon, float lat_ant, float lon_ant) {
  return 90.0 * grado_to_radian - (2 * M_PI + atan2(lon - lon_ant, lat - lat_ant));
}

/*
   Método para imprimir las coordenadas en formato [LAT_KAL,LON_KAL,LAT_ORI,LON_ORI,MILLIS]
*/
void imprimir_json_comp(float lat_kal, float lon_kal) {
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

