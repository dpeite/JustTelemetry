import serial
import matplotlib.pyplot as plt
from drawnow import *

lat = []
lon = []

plt.ion()
cnt=0
imprimir = 0
first = True

serialArduino = serial.Serial('COM4', 38400)

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
    if first:
        import time 
    time.sleep(8)
    first = False

    valueRead = serialArduino.readline()

    print (valueRead) #Para poder copiar lo que sale por terminal a un fichero

    asf = valueRead.split("[")
    try:
        coord = asf[1].split(",")
	
        #check if valid value can be casted
        try:
            lat1 = float(coord[0])
            lon1 = float(coord[1]) 

            lat.append(lat1)
            lon.append(lon1)
            lat.pop(0)
            lon.pop(0)
            drawnow(plotValues)
        except ValueError as exc:
            print ("Invalid! cannot cast")
            print (exc)
    except:
        pass
