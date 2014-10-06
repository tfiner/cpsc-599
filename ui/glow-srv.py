#!/usr/bin/env python

from flask import Flask
from flask import request
from flask import send_file

from PIL import Image
from StringIO import StringIO

import os
import json
import urllib
import base64

app = Flask(__name__, static_folder='assets')


def imgToString(pil_img):
    img_io = StringIO()
    pil_img.save(img_io, 'JPEG', quality=70)
    img_io.seek(0)
    return img_io.getvalue()
    # return send_file(img_io, mimetype='image/jpeg')


@app.route('/')
def root():
    return app.send_static_file('glow.html')    

@app.route('/server/status')
def server_status():
    return json.dumps({'success':True}), 200, {'ContentType':'application/json'}

@app.route('/glow/setScene')
def set_scene():
    query=urllib.unquote(request.query_string).decode('utf8') 
    # print "request query:", query
    print "request query (json):", json.dumps(query)
    return json.dumps({'success':True}), 200, {'ContentType':'application/json'}


@app.route('/glow/render')
def render():
    query=urllib.unquote(request.query_string).decode('utf8') 
    # print "request query:", query
    print "request query (json):", json.dumps(query)

    img = Image.new("RGB", (512, 512), "red")
    imgStr = imgToString(img)
    b64Img = base64.b64encode(imgStr)
    # return serve_pil_image(img)
    return json.dumps(
        {'image':b64Img}), 200, {'ContentType':'application/json'}


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