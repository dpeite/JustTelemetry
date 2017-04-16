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

def cortar_varios(lat1, lon1, lat2, lon2, ID, sensores):
    with open("app/static/data/sesiones/"+ID+"/"+sensores[0].split("_")[0]+".json") as data_file:
        sensor_0 = json.load(data_file)

    with open("app/static/data/sesiones/"+ID+"/"+sensores[1].split("_")[0]+".json") as data_file:
        sensor_1 = json.load(data_file)

    key = sensores[0].split("_")[1]
    datos_sensores = {}
    datos_sensores[key] = {}
    for elements in sensor_0[key]:
        if "data" in elements:
            datos = []
            dentro = False
            for index, element in enumerate(sensor_0[key][elements]):
                if (element[2] == lat1 and element[3] == lon1) or dentro:
                    dentro = True
                    element1 = list(element)
                    element1.insert(1, sensor_1[sensores[1].split("_")[1]][elements][index][1])
                    element1[0],element1[2]=element1[2],element1[0]
                    datos.append(element1)
                if (element[2] == lat2 and element[3] == lon2):
                    break;
          
            datos_sensores[key][elements] = datos

    if len(datos) == 0:
        for elements in sensor_0[key]:
            if "data" in elements:
                datos = []
                dentro = False
                for index, element in enumerate(sensor_0[key][elements]):
                    if (element[2] == lat2 and element[3] == lon2) or dentro:
                        dentro = True
                        element1 = list(element)
                        element1.insert(1, sensor_1[sensores[1].split("_")[1]][elements][index][1])
                        element1[0],element1[2]=element1[2],element1[0]
                        datos.append(element1)
                    if (element[2] == lat1 and element[3] == lon1):
                        break;

                datos_sensores[key][elements] = datos

    return datos_sensores

    
    # for key in sensor_0.keys():
    #     for elements in sensor_0[key]:
    #         if "data" in elements:
    #             datos = []
    #             dentro = False
    #             for index, element in enumerate(sensor_0[key][elements]):
    #                 if (element[2] == lat1 and element[3] == lon1) or dentro:
    #                     dentro = True
    #                     # print sensor_1[key][elements][index]
    #                     print element
    #                     element1 = list(element)
    #                     element1.insert(1, sensor_1[key][elements][index][1])
    #                     element1[0],element1[2]=element1[2],element1[0]
    #                     datos.append(element1)
    #                     print element
    #                     print element1
    #                 if (element[2] == lat2 and element[3] == lon2):
    #                     break;
                        
    #             del sensor_0[key][elements]
    #             sensor_0[key][elements] = datos
                                                                                                                            
    # return sensor_0
