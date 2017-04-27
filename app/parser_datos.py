from json import load, dump
import os
import zipfile
import random
import json

def obtenerZip():
    with open('app/datos_in.json') as datos_ind:
        d = load(datos_ind)

    data_sensor_unidimensional = []

    saltar = False

    json_info = {}
    json_big = {}
    json_data = {}
    array_data = []

    def multi_sensor(n, nombre):

        impr = []
        json_sensor = {}
        for i in range(n):
            json_datos = {}
            array_data = []
            json_sensor_data = {}
            for medida in d:
                cont = 0
                for pos in medida["values"][nombre]:
                    if (cont == i) :
                        legend = pos
                        impr.append(medida["datetime"])
                        impr.append(medida["values"][nombre][pos])
                        impr.append("lat")
                        impr.append("lon")
                        
                        array_data.append(impr)
                        json_datos = array_data
                        impr = []
                    cont += 1

            json_sensor_data["data"] = json_datos
            json_sensor_data["label"] = legend
            json_sensor[legend] = json_sensor_data

        with open("app/" + nombre + ".json", 'w') as outfile:
            dump(json_sensor, outfile)

    # Esto nos da los nombres de los sensores
    for y in d[0]["values"]:
        for z in d:
            data_sensor_unidimensional.append(z["datetime"])

            try:
                multi_sensor(len(z["values"][y]), y)
                saltar = True
                break
            except:
                saltar = False
                data_sensor_unidimensional.append(z["values"][y])
                data_sensor_unidimensional.append("lat")
                data_sensor_unidimensional.append("lon")

            array_data.append(data_sensor_unidimensional)
            data_sensor_unidimensional = []

        json_data["data"] = array_data
        json_data["label"] = y
        json_big[y] = json_data
        

        if not saltar:
            with open("app/" + y + ".json", 'w') as outfile:
                dump(json_big, outfile)

        array_data = []
        json_data = {}
        json_big = {}

    import time
    fjson_info = {}
    fjson_info["fecha"] = time.strftime("%d-%m-%Y")
    fjson_info["descripcion"] = "Prueba realizada en " + "circuito" + " usando un " + "coche" + "(" + "clase" + ")"
    fjson_info["nombre"] = "circuito" + "-pCars"
    fjson_info["metros"] = 2 * 1000

    id_sesion = random.randint(1000, 10000)

    while (True):

      if not os.path.exists(str(id_sesion) + ".zip"):
        #print (str(id_sesion) + ".zip")
        fjson_info["id"] = id_sesion
        break;
      else:
        id_sesion = random.randint(1000, 10000)

    with open('app/info.json', 'w') as outfile:
            json.dump(fjson_info, outfile)


    dirname = "app/static/data/sesiones"
    with zipfile.ZipFile(str(fjson_info["id"]) + ".zip", 'w') as zf:
        zf.write(os.path.join("app/", "acelerator.json"), "acelerator.json")
        zf.write(os.path.join("app/", "maf.json"), "maf.json")
        zf.write(os.path.join("app/", "rpm.json"), "rpm.json")
        zf.write(os.path.join("app/", "info.json"), "info.json")
        return zf;
