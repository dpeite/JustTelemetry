import json
import os
from app import app
from flask import render_template, make_response

@app.route("/")
@app.route("/index.html")
def index():
    user = {"nickname":"asdf"}
    resp = make_response(render_template("index.html", title="Home", user=user))
    resp.cache_control.no_cache = True
    return resp

@app.route("/tiempo.html")
def tiempo():
    resp = make_response(render_template("tiempo.html"))
    resp.cache_control.no_cache = True
    return resp

@app.route("/posicion.html")
def posicion():
    resp = make_response(render_template("posicion.html"))
    resp.cache_control.no_cache = True
    return resp

@app.route("/sensores.html")
def sensores():
    resp = make_response(render_template("sensores.html"))
    resp.cache_control.no_cache = True
    return resp

@app.route("/tablas.html")
def tablas():
    resp = make_response(render_template("tables.html"))
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
    
    resp = make_response(render_template("sesiones.html", posts=posts))
    resp.cache_control.no_cache = True
    
    return resp

@app.route("/importar-exportar.html")
def importar_exportar():
    resp = make_response(render_template("importar-exportar.html"))
    resp.cache_control.no_cache = True
    return resp
