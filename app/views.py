import json
import os
from os import listdir
import shutil
from app import app
from flask import render_template, make_response, request, send_file, jsonify
from io import BytesIO
import zipfile
import ast

@app.route("/")
@app.route("/index.html")
def index():
    user = {"nickname":"asdf"}
    resp = make_response(render_template("index.html", title="Home", user=user))
    resp.cache_control.no_cache = True
    return resp

@app.route("/tiempo.html")
def tiempo():
    ID = str(request.cookies.get("id"))
    try:
        path, dirs, files = os.walk("app/static/data/sesiones/"+ID+"/vueltas").next()
        vueltas = len(files)
    except:
        vueltas = 0
        pass
    resp = make_response(render_template("tiempo.html", vueltas=vueltas))
    resp.cache_control.no_cache = True
    return resp

@app.route("/posicion.html")
def posicion():
    ID = str(request.cookies.get("id"))
    try:
        path, dirs, files = os.walk("app/static/data/sesiones/"+ID+"/vueltas").next()
        vueltas = len(files)
    except:
        vueltas = 0
        pass
    resp = make_response(render_template("posicion.html", vueltas=vueltas))
    resp.cache_control.no_cache = True
    return resp

@app.route("/relacion.html")
def relacion():
    ID = str(request.cookies.get("id"))
    try:
        path, dirs, files = os.walk("app/static/data/sesiones/"+ID+"/vueltas").next()
        vueltas = len(files)
    except:
        vueltas = 0
        pass
    sensores = consultar_sensores(ID)
    resp = make_response(render_template("relacion.html", vueltas=vueltas, sensores=sensores))
    resp.cache_control.no_cache = True
    return resp

def consultar_sensores(id):
    sensores =  ["ruedas.json", "amortiguadores.json", "volante.json", "acelerador.json"]
    sensores_disponibles = []
    archivos = listdir("app/static/data/sesiones/"+id)
    for sensor in sensores:
        if sensor in archivos:
            sensores_disponibles.append(sensor.split(".")[0])
    return sensores_disponibles


@app.route("/tablas.html")
def tablas():
    resp = make_response(render_template("tablas.html"))
    resp.cache_control.no_cache = True
    return resp

@app.route("/sesiones.html")
def sesiones():
    # with open('app/static/data/sesiones.json') as data_file:
    #     posts = json.load(data_file)
    posts = []
    for path, dirs, files in os.walk("app/static/data/sesiones/"):
        for d in dirs:
            with open("app/static/data/sesiones/"+d+"/info.json") as data_file:
                info = json.load(data_file)
                posts.append(info)
        break
    
    resp = make_response(render_template("sesiones.html", posts=posts))
    resp.cache_control.no_cache = True
    
    return resp

@app.route("/controlremoto.html")
def importar_exportar():
    resp = make_response(render_template("controlremoto.html"))
    resp.cache_control.no_cache = True
    return resp

##################Local endpoints#######################
@app.route("/cortar_vueltas")
def cortar_vueltas():
    lat = float(request.args.get("lat"))
    lon = float(request.args.get("lon"))
    ID = str(request.args.get("id"))
    try:
        import vueltas
        vueltas.cortar(lat, lon, ID)
        return "", 200
    except Exception as exc:
        print exc
        return "", 500

@app.route("/cortar_json")
def cortar_json():
    lat1 = float(request.args.get("lat1"))
    lon1 = float(request.args.get("lon1"))
    lat2 = float(request.args.get("lat2"))
    lon2 = float(request.args.get("lon2"))
    ID = str(request.args.get("id"))
    sensor = str(request.args.get("sensor"))
    dist = str(request.args.get("dist")).capitalize()
    dist =  ast.literal_eval(dist)
    try:
        import tramo
        sensores = sensor.split("-")
        if len(sensores) == 2:
            return jsonify(tramo.cortar_varios(lat1, lon1, lat2, lon2, ID, sensores)), 200
        else:
            if "fuerzas" in sensor:
                dist = False
            return jsonify(tramo.cortar(lat1, lon1, lat2, lon2, ID, sensor, dist)), 200
    except Exception as exc:
        print exc
        return "", 500

@app.route("/borrar_vuelta")
def borar_vuelta(): 
    shutil.rmtree('app/static/data/sesiones/' + str(request.args.get("id")) + '/vueltas')
    return "", 200

@app.route("/borrar_sesion")
def borrar_sesion():
    shutil.rmtree('app/static/data/sesiones/' + str(request.args.get("id")))
    return "", 200

@app.route("/descargar_sesion")
def descargar_sesion():
    # zip =shutil.make_archive("app/static/data/sesiones/" + str(request.args.get("id")), 'zip', 'app/static/data/sesiones/'+ str(request.args.get("id")))
    # return send_file(zip)
    memory_file = BytesIO()
    with zipfile.ZipFile(memory_file, 'w') as zf:
        for dirname, subdirs, files in os.walk("app/static/data/sesiones/" + str(request.args.get("id"))):
            print subdirs
            if not "vueltas" in dirname:
                for filename in files:
                    zf.write(os.path.join(dirname, filename), filename)
            else:
                for filename in files:
                    zf.write(os.path.join(dirname, filename), "vueltas/"+filename)
    memory_file.seek(0)
    return send_file(memory_file, attachment_filename= str(request.args.get("id"))  +'.zip', as_attachment=True)

@app.route("/editar_datos", methods=["POST"])
def editar_datos():
    ID = request.values["datos[id]"]
    datos = request.values
    print datos
    editar_datos_fichero(ID,datos)
    return "", 200

def editar_datos_fichero(ID, valores, form=None):
    with open("app/static/data/sesiones/"+ID+"/info.json") as data_file:
        datos = json.load(data_file)

    for key, value in valores.iteritems():
        if not form:
            datos[key.split("[")[1].split("]")[0]] = value
        else:
            try:
                #print key
                datos[key] = value
            except exc as Exception:
                print exc
      
    with open("app/static/data/sesiones/"+ID+"/info.json", "w") as data_file:
        json.dump(datos, data_file)

@app.route("/get_sensores")
def get_sensores(): 
    ID = str(request.args.get("id"))
    sensor = str(request.args.get("sensor"))
    with open("app/static/data/sesiones/"+ID+"/"+sensor+".json") as data_file:
        info = json.load(data_file)

    return jsonify(info.keys())

@app.route("/upload_sesion", methods=["POST"])
def upload_sesion():
    # print request.form["descripcion"]
    file = request.files['file-0']
    file_name = file.filename.split(".")[0]
    if allowed_file(file.filename):
        if not os.path.exists("app/static/data/sesiones/"+file_name):
            os.makedirs("app/static/data/sesiones/"+file_name)
            zip_ref = zipfile.ZipFile(file)
            zip_ref.extractall("app/static/data/sesiones/"+file_name)
            zip_ref.close()
        else:
            return "La sesion ya existe con ese id", 500
        print file_name
        editar_datos_fichero(file_name, request.values, form=True)
        return "", 200
    else:
        return "Extension no permitida", 500

def allowed_file(filename):
    ALLOWED_EXTENSIONS = set(['zip'])
    return '.' in filename and \
        filename.rsplit('.', 1)[1].lower() in ALLOWED_EXTENSIONS

@app.route("/upload_sesion_ip", methods=["POST"])
def upload_sesion_ip():

    for key, value in request.values.iteritems():
        if (key == "ip"):
            ip = value

    # TODO:Obtener JSON usando la IP


    import parser_datos
    file = parser_datos.obtenerZip()
    file_name = file.filename.split(".")[0]

    if allowed_file(file.filename):
        if not os.path.exists("app/static/data/sesiones/"+file_name):
            os.makedirs("app/static/data/sesiones/"+file_name)
            zip_ref = zipfile.ZipFile(file.filename)
            zip_ref.extractall("app/static/data/sesiones/"+file_name)
            zip_ref.close()

            os.remove(file.filename) # Eliminamos el ZIP

            for item in os.listdir("app/"):
                if item.endswith(".json") and not ("datos_in" in item):
                    os.remove("app/" + item)
        else:
            return "La sesion ya existe con ese id", 500
        print file_name
        editar_datos_fichero(file_name, request.values, form=True)
        return "", 200
    else:
        return "Extension no permitida", 500

    return "", 200

@app.route("/info_sesiones")
def info_sesiones():
    source = "app/static/data/sesiones/"
    files = os.listdir(source)

    distancia = 0
    numero_sesiones = 0
    tiempo_total = 0
    array_sesiones = []
    lista_sesiones = {}
    for f in files:

        with open(source + f + '/info.json') as new_file:
            info_json = json.load(new_file)

        distancia += int(info_json["metros"])
        numero_sesiones += 1

        # Contamos el tiempo
        m, s = info_json["tiempo"].split(' ')
        tiempo_total += int(m[:-1]) * 60 + int(s[:-1])

        # Para contar el nmero de sesiones
        if info_json["fecha"].split()[0] in lista_sesiones:
            lista_sesiones[info_json["fecha"].split()[0]] += 1
        else:
            lista_sesiones[info_json["fecha"].split()[0]] = 1

    print lista_sesiones
    array_sesiones = []
    for x in lista_sesiones:
        sesion = {}
        sesion["fecha"] = x
        sesion["numero"] = lista_sesiones[x]
        array_sesiones.append(sesion)

    print numero_sesiones
    print array_sesiones

    fichero = {}
    fichero["numero"] = numero_sesiones
    fichero["distancia"] = distancia

    m, s = divmod(tiempo_total, 60)
    h, m = divmod(m, 60)
    str_tiempo = "%d:%02d:%02d" % (h, m, s)
    fichero["tiempo"] = str_tiempo
    fichero["errores"] = 3
    fichero["sesiones"] = array_sesiones

    with open('app/static/data/dash.json', 'wb') as new_file:
        json.dump(fichero, new_file)

    return "", 200
