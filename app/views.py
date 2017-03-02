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
    posts = [  # fake array of posts
        {
            'id': '0',
            'fecha':'20/02/2017 11:50',
            'nombre': 'Prueba frenos',
            'tiempo': '1m 2s',
            'metros': '100'
        },
        {
            'id': '1',
            'fecha': '20/02/2017 11:00',
            'nombre': 'Prueba motor',
            'tiempo': '2m 24s',
            'metros': '500'
        },
    ]
    resp = make_response(render_template("sesiones.html", posts=posts))
    resp.cache_control.no_cache = True
    
    return resp

@app.route("/importar-exportar.html")
def importar_exportar():
    resp = make_response(render_template("importar-exportar.html"))
    resp.cache_control.no_cache = True
    return resp
