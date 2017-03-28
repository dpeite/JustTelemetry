import json

def cortar(lat1, lon1, lat2, lon2, ID, sensor):
    with open("app/static/data/sesiones/"+ID+"/"+sensor+".json") as data_file:
        info = json.load(data_file)

    for key in info.keys():
        for elements in info[key]:
            if "data" in elements:
                datos = []
                dentro = False
                for index, element in enumerate(info[key][elements]):
                    if (element[2] == lat1 and element[3] == lon1) or dentro:
                        dentro = True
                        datos.append(element)
                    if (element[2] == lat2 and element[3] == lon2):
                        break;
 
                del info[key][elements]
                info[key][elements] = datos
                

    if len(datos) == 0:
        with open("app/static/data/sesiones/"+ID+"/"+sensor+".json") as data_file:
            info = json.load(data_file)

        for key in info.keys():
            for elements in info[key]:
                if "data" in elements:
                    datos = []
                    dentro = False
                    for index, element in enumerate(info[key][elements]):
                        print element[2]
                        if (element[2] == lat2 and element[3] == lon2) or dentro:
                            print "Dentro"
                            dentro = True
                            datos.append(element)
                        if (element[2] == lat1 and element[3] == lon1):
                            break;

                    del info[key][elements]
                    info[key][elements] = datos

    return info
