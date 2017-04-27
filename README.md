# JustTelemetry

http://justtelemetry.ddns.net/

### Descripción
 En el marco de una colaboración con la escudería de Formula Student de la Universidad de Vigo (UVigo Motorsport) ofrecemos un sistema de telemetría para su vehículo. Concretamente, hemos desarrollado un sistema de posicionamiento mediante GPS + Unidad de medición inercial y una interfaz web para la visualización de los datos recogidos por los sensores.

### Partes del proyecto

#### Sistema de posicionamiento (https://github.com/dpeite/JustTelemetry/tree/posicionamiento)
 Nuestro sistema de posicionamiento se basa en la fusión de los datos de un módulo GPS y las lecturas de un acelerómetro para obtener el recorrido del vehiculo a lo largo de la trazada. Esa fusión se realiza mediante la implementación de un filtro de Kalman, y actualmente hemos conseguido disminuir el error de la posición (que en el caso de usar sólo el GPS podría llegar a los diez metros) a menos de 3m.
 
 La información obtenida se envía mediante el puerto serie a una Raspberry Pi situada en el coche, que se encarga de almacenarla en una base de datos junto con las lecturas obtenidas por los sensores del vehiculo.
 
#### Comunicación
 Una vez el vehículo ha terminado la sesión de prácticas el usuario que quiera acceder a la información se conectará a la red WiFi creada por la Raspberry y accederá al servidor web que hay en la misma.
 
#### Interfaz web (https://github.com/dpeite/JustTelemetry/tree/web)

 La interfaz web consta de varias partes:
##### Página inicial
 En la página principal se muestra un breve resumen de la información de las sesiones, la distribución de las mismas a lo largo del tiempo y un panel de notificaciones.
##### Gráficas
 Las gráficas de nuestra interfaz muestran las lecturas de los sensores, y se pueden ver respecto al tiempo, a la distancia recorrida o a otros sensores. Además también se puede ver el recorrido del coche y ver los datos en un punto concreto de la trazada, así como seleccionar sólo un sector concreto de la pista. 
##### Tablas
 Aquí se muestran los datos en bruto de los sensores, para una consulta más rápida.
##### Sesiones
 En esta parte se muestra un listado de las sesiones de pruebas realizadas, así como la posibilidad de importar, editary borrar sesiones. Ademas se ofrece la posibilidad de indicar el punto de inicio del circuito para así separar el recorrido en vueltas.
