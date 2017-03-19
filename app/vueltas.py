import json
import os
import shutil

def cortar(lat, lon, ID):
    # lat = 37.0913887
    # lon = -42.37863922
    # lat = 33.7829132 
    # lon = -38.87612533
    vuelta = []
    vueltas = []
    print type(lat)
    with open("app/static/data/sesiones/"+ID+"/trazada.json") as data_file:
        info = json.load(data_file)
    sig = False
    for coord in info:
        if (lat-2) <= coord[0] <= (lat+2) and (lon-2) <= coord[1] <= (lon+2):
            if not sig:
                vueltas.append(vuelta[:])
                del vuelta[:]
            # print coord
            sig = True
        else:
            sig = False
        vuelta.append(coord)
    vueltas.append(vuelta[:])

    print len(vueltas)
    filename = "app/static/data/sesiones/"+ID+"/vueltas/"
    print os.path.dirname(filename)
    if os.path.exists(os.path.dirname(filename)):
        shutil.rmtree(filename)
    
    try:
        os.makedirs(os.path.dirname(filename))
    except OSError as exc: # Guard against race condition
        if exc.errno != errno.EEXIST:
            raise

    for vuelta, coords in enumerate(vueltas):
        with open("app/static/data/sesiones/"+ID+"/vueltas/vuelta_"+str(vuelta)+".json", 'w') as outfile:
            json.dump(coords, outfile)
