from app import app
from flask import render_template, make_response

@app.route("/")
@app.route("/index.html")
def index():
    user = {"nickname":"asdf"}
    resp = make_response(render_template("index.html", title="Home", user=user))
    resp.cache_control.no_cache = True
    return resp

@app.route("/flot.html")
def flot():
    resp = make_response(render_template("flot.html"))
    resp.cache_control.no_cache = True
    return resp

@app.route("/morris.html")
def morris():
    resp = make_response(render_template("morris.html"))
    resp.cache_control.no_cache = True
    return resp

@app.route("/tables.html")
def tables():
    resp = make_response(render_template("tables.html"))
    resp.cache_control.no_cache = True
    return resp

@app.route("/forms.html")
def forms():
    resp = make_response(render_template("forms.html"))
    resp.cache_control.no_cache = True
    return resp

@app.route("/panels-wells.html")
def panels():
    resp = make_response(render_template("panels-wells.html"))
    resp.cache_control.no_cache = True
    return resp

@app.route("/buttons.html")
def buttons():
    resp = make_response(render_template("buttons.html"))
    resp.cache_control.no_cache = True
    return resp

@app.route("/notifications.html")
def notifications():
    resp = make_response(render_template("notifications.html"))
    resp.cache_control.no_cache = True
    return resp

@app.route("/typography.html")
def typography():
    resp = make_response(render_template("typography.html"))
    resp.cache_control.no_cache = True
    return resp

@app.route("/icons.html")
def icons():
    resp = make_response(render_template("icons.html"))
    resp.cache_control.no_cache = True
    return resp

@app.route("/grid.html")
def grid():
    resp = make_response(render_template("grid.html"))
    resp.cache_control.no_cache = True
    return resp

@app.route("/blank.html")
def blank():
    resp = make_response(render_template("blank.html"))
    resp.cache_control.no_cache = True
    return resp

@app.route("/login.html")
def login():
    resp = make_response(render_template("login.html"))
    resp.cache_control.no_cache = True
    return resp
