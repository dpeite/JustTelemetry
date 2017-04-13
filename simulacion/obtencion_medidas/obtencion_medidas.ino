#include <TinyGPS.h>
#include <MPU9250.h>

// Dimensión del tamaño del estado
#define n 6

// Vector de medidas
float z[n];

// Para corregir los ejes
float acelx, acely;

// Para calcular la orientación
long t_inicial, t_final;

//Variables del GPS
float lat_ini, lon_ini, flat_ant, flon_ant, flat, flon, velocidad;
float angulo, orientacion, delta_t;
unsigned long age, distancia;

// Constantes
const float grado_to_radian = 0.0174533;
const float g_to_ms2 = 9.80665; // m/s
const float km_to_ms = 0.277778;

// Creamos las instancias de la librerías
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
    velocidad = gps.f_speed_kmph() * km_to_ms;

    orientacion = gps.course_to(flat_ant, flon_ant, flat, flon);
    angulo = gps.course_to(lat_ini, lon_ini, flat, flon);

    myIMU.readAccelData(myIMU.accelCount);  // Read the x/y/z adc values
    acelx = (float)myIMU.accelCount[0] * myIMU.aRes * g_to_ms2;
    acely = (float)myIMU.accelCount[1] * myIMU.aRes * g_to_ms2;

    // Comprobamos que nos hemos movido del origen
    if (distancia != 0) {
      z[0] = distancia * sin(grado_to_radian * angulo); 
      z[1] = distancia * cos(grado_to_radian * angulo);
      z[2] = acelx * cos((90 - orientacion) * grado_to_radian) + acely * sin((90 - orientacion) * grado_to_radian);
      z[3] = acely * cos((90 - orientacion) * grado_to_radian) - acelx * sin((90 - orientacion) * grado_to_radian);
      z[4] = velocidad * sin(grado_to_radian * orientacion);
      z[5] = velocidad * cos(grado_to_radian * orientacion);
    }

    imprimir((float*) z, delta_t);

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
   Método para imprimir las lecturas de los sensores en formato [LAT, LON, AX, AY, VX, VY, DELTA_T]
*/
void imprimir(float* z, float t) {
  Serial.print('[');
  Serial.print(z[0], 8);
  Serial.print(',');
  Serial.print(z[1], 8);
  Serial.print(',');
  Serial.print(z[2], 8);
  Serial.print(',');
  Serial.print(z[3], 8);
  Serial.print(',');
  Serial.print(z[4], 8);
  Serial.print(',');
  Serial.print(z[5], 8);
  Serial.print(',');
  Serial.print(t);
  Serial.print(']');
  Serial.println(',');
}


