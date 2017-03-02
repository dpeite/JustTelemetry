#include "TinyGPS.h"

#define RXPIN 3
#define TXPIN 2

TinyGPS gps;
SoftwareSerial nss(RXPIN, TXPIN);

unsigned long fix_age;
float flat, flon;
float lat1, lat2;
float lon1, lon2;
float a;
float c;
float d;
const R = 6367000;

void setup() {
  // put your setup code here, to run once:

  // Me parese que el while aquí es estúpido (#dato)
  while (nss.available()) {
    int c = nss.read();
    if (gps.encode(c)) {
      // returns +/- latitude/longitude in degrees
      gps.f_get_position(&flat, &flon, &fix_age);
      lat2 = flat;  // inicializamos los valores de lat2 y lon2, así en la primera iteración
      lon2 = flon;  //se pasan a lat1 y lon2
      //delay(5000);
      break;
    }
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  while (nss.available()) {
    int c = nss.read();
    if (gps.encode(c)) {
      // returns +/- latitude/longitude in degrees
      gps.f_get_position(&flat, &flon, &fix_age);
      //float falt = gps.f_altitude(); // +/- altitude in meters
      //float fmps = gps.f_speed_mps(); // speed in m/sec
      //float fkmph = gps.f_speed_kmph(); // speed in km/hr
      lat1 = lat2;  // actualizamos los valores del estado anterior
      lon1 = lon2;
      lat2 = flat;  // actualizamos los valores del estado actual
      lon2 = flon;

      // ecuación de haversine:
      dlon = lon2 - lon1;
      dlat = lat2 - lat1;
      a = sin ^ 2(dlat / 2) + cos(lat1) * cos(lat2) * sen ^ 2(dlon / 2);
      c = 2 * asin(min(1, sqrt(a)));
      d = R * c;
      delay(10000);
    }
  }
}
