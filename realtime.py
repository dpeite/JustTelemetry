import serial
import matplotlib.pyplot as plt
from drawnow import *

lat = []
lon = []

plt.ion()
cnt=0
imprimir = 0

serialArduino = serial.Serial('/dev/ttyACM0', 38400)

def plotValues():
    plt.title('Trazada')
    plt.grid(True)
    plt.ylabel('Eje y')
    plt.xlabel('Eje x')
    plt.plot(lat, lon , 'rx-', label='values')
    plt.legend(loc='upper right')

#pre-load dummy data
for i in range(0,5000):
    lat.append(0)
    lon.append(0)

while True:
    while (serialArduino.inWaiting()==0):
        pass
    valueRead = serialArduino.readline()

    print valueRead #Para poder copiar lo que sale por terminal a un fichero

    asf = valueRead.split("[")
    try:
        coord = asf[1].split(",")

        #check if valid value can be casted
        try:
            lat1 = float(coord[0])
            lon1 = float(coord[1]) 

            if lat1 == 0.00000000:
                imprimir == 1
            else:
                if imprimir == 1:
                    lat.append(lat1)
                    lon.append(lon1)
                    lat.pop(0)
                    lon.pop(0)
                    drawnow(plotValues)
        except ValueError as exc:
            print "Invalid! cannot cast"
            print exc
    except:
        pass
