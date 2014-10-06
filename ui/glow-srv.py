#!/usr/bin/env python

from flask import Flask
from flask import request
from flask import send_file

from StringIO import StringIO

import os
import json
import urllib
import base64
import tempfile

import glow


app = Flask(__name__, static_folder='assets')


cmdLine = "glow --input={0} --output={1} --verbose"
curSceneFile = ""


@app.route('/')
def root():
    return app.send_static_file('glow.html')    

@app.route('/server/status')
def server_status():
    return json.dumps({'success':True}), 200, {'ContentType':'application/json'}

@app.route('/glow/setScene')
def set_scene():
    global curSceneFile

    print "-" * 80
    print "request query:", request.query_string
    query = urllib.unquote(request.query_string).decode('utf8') 

    print "-" * 80
    print "request query (unquoted):", query

    print "-" * 80
    print "request query (json):", json.dumps(query, indent=4)

    print "-" * 80

    handle, name = tempfile.mkstemp(suffix='.json', prefix='glow-')
    print "curScene:", name
    outFile = os.fdopen(handle, "w")
    outFile.write(query)
    # json.dump(query, outFile, indent=4)
    curSceneFile = name

    return json.dumps({'success':True}), 200, {'ContentType':'application/json'}


@app.route('/glow/render')
def render():
    query=urllib.unquote(request.query_string).decode('utf8') 
    # print "request query:", query
    print "request query (json):", json.dumps(query)

    (handle, outputName) = tempfile.mkstemp(suffix='.png', prefix='glow-')

    cmdLineArgs = "glow --input={0} --output={1} --verbose".format(curSceneFile, outputName)
    print "Calling glow:", cmdLineArgs

    ret = glow.run(cmdLineArgs.split())
    retVal = json.dumps({'glow':ret}), 500, {'ContentType':'application/json'}
    if ret == 0:
        b64Img = StringIO()
        with open(outputName) as img:
            base64.encode(img, b64Img)
            retVal = json.dumps({'image':b64Img.getvalue()}), 200, {'ContentType':'application/json'}

    return retVal


# TODO:1 these two handlers might not be necessary.
@app.route('/js/<path:path>')
def static_js_proxy(path):
    return app.send_static_file(os.path.join('js', path))

@app.route('/css/<path:path>')
def static_css_proxy(path):
    return app.send_static_file(os.path.join('css', path))

@app.route('/font/<path:path>')
def static_font_proxy(path):
    return app.send_static_file(os.path.join('font', path))

@app.route('/images/<path:path>')
def static_images_proxy(path):
    return app.send_static_file(os.path.join('images', path))


if __name__ == "__main__":
    app.debug = True
    app.run(port=5001)