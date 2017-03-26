#include <TinyGPS.h>

TinyGPS gps;

#define n_coords 44

const float grado_to_radian = 0.0174533;
float z[2];
//float coords[22] = {42.167845, -8.690422, 42.168022, -8.690412, 42.168121, -8.690457, 42.168150, -8.690517, 42.168137, -8.690597, 42.168062, -8.690655, 42.167836, -8.690653, 42.167684, -8.690546, 42.166932, -8.689408, 42.166970, -8.689287, 42.167071, -8.689289};

float coords[n_coords] = {42.167301, -8.689617, 42.167793, -8.690348,
                  42.167846, -8.690400, 42.167899, -8.690409,
                  42.168013, -8.690413,
                  42.168079, -8.690420,
                  42.168130, -8.690460,
                  42.168155, -8.690515,
                  42.168144, -8.690591,
                  42.168104, -8.690633,
                  42.168060, -8.690656,
                  42.167873, -8.690662,
                  42.167811, -8.690647,
                  42.167752, -8.690617,
                  42.167696, -8.690570,
                  42.166959, -8.689467,
                  42.166939, -8.689400,
                  42.166945, -8.689335,
                  42.166972, -8.689284,
                  42.167029, -8.689267,
                  42.167081, -8.689294,
                  42.167301, -8.689617,
                 };
void setup()
{
  Serial.begin(38400);


  float lat_ini = 42.167301, lon_ini = -8.689617;
  float flat = 42.167845, flon = -8.690422;

  for (int i = 0; i < n_coords ; i += 2) {
    float angulo =  TinyGPS::course_to(lat_ini, lon_ini, coords[i], coords[i + 1]);
    unsigned long distancia = (unsigned long)TinyGPS::distance_between(lat_ini, lon_ini, coords[i], coords[i + 1]);

    z[0] = -16 + distancia * cos(grado_to_radian * angulo) * (-1); // Para invertir la imagen
    z[1] = -16 + distancia * sin(grado_to_radian * angulo);

    Serial.print(z[0]);
    Serial.print(",");
    Serial.println(z[1]);
  }
}

void loop()
{

}
