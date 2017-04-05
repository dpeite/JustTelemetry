# Dibuja la trazada usando solamente datos del acelerómetro

 Están implementadas dos formas de hacerlo. La primera es usando tal cual las medidas del acelerómetro, mientras que la segunda consiste en aplicar un filtro de Kalman a las medidas de la aceleración y con esas medidas corregidas calcular la posición.

  El programa envía a través del Serial un string de la forma `[px_acel, py_acel, px_kalman, py_kalman, delta_t]`