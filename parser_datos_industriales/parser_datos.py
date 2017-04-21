from json import load, dump

with open('datos_in.json') as datos_ind:
    d = load(datos_ind)

data_sensor_unidimensional = []

saltar = False

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

    with open(nombre + ".json", 'w') as outfile:
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
        with open(y + ".json", 'w') as outfile:
            dump(json_big, outfile)

    array_data = []
    json_data = {}
    json_big = {}