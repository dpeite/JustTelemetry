import json
import os
import shutil
from app import app
from flask import render_template, make_response, request, send_file
from io import BytesIO
import zipfile

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
    resp = make_response(render_template("posicion.html"))
    resp.cache_control.no_cache = True
    return resp

@app.route("/sensores.html")
def sensores():
    ID = str(request.cookies.get("id"))
    try:
        path, dirs, files = os.walk("app/static/data/sesiones/"+ID+"/vueltas").next()
        vueltas = len(files)
    except:
        vueltas = 0
        pass
    resp = make_response(render_template("sensores.html", vueltas=vueltas))
    resp.cache_control.no_cache = True
    return resp

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
    try:
        import tramo
        from flask import jsonify
        return jsonify(tramo.cortar(lat1, lon1, lat2, lon2, ID, sensor)), 200
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
