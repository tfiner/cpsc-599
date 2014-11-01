#!/usr/bin/env python

from flask import Flask
from flask import request
from flask import send_file
from flask import abort
from flask.helpers import send_from_directory

from StringIO import StringIO
from PIL import Image

import os
import json
import urllib
import base64
import tempfile
import hashlib

import glow


app = Flask(__name__, static_folder='assets')


cmdLine = "glow --input={0} --output={1} --verbose"
curSceneFile = ""
curSceneHash = ""
curNoisePreviewDir = ""

@app.route('/')
def root():
    return app.send_static_file('glow.html')    

@app.route('/server/status')
def server_status():
    return json.dumps({'success':True}), 200, {'ContentType':'application/json'}

# @app.route('/glow/setScene')
def set_scene():
    global curSceneFile, curSceneHash

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

    hasher = hashlib.md5()
    hasher.update(query)
    curSceneHash = hasher.hexdigest()

    # return json.dumps({'success':True}), 200, {'ContentType':'application/json'}


@app.route('/glow/render')
def render():
    set_scene()

    # query=urllib.unquote(request.query_string).decode('utf8') 
    # # print "request query:", query
    # print "request query (json):", json.dumps(query)

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


# /glow/render/tile/tiles/1/2_2.png
@app.route('/glow/render/tiles/<int:zoom>/<path:tileId>')
def render_tile(zoom, tileId):
    print "Request for tile: {1} at zoom {0}".format(zoom, tileId)

    # Generate a consistent name based upon the tile request.
    # Identical future requests result in the same name.
    # This is the basis for the tile cache.
    col,row = tileId.split('.')[0].split('_')

    x0 = (int(col)-1) * 256
    x1 = x0 + 256

    # y is inverted (the pano code assumes y is down...)
    row = 8 - int(row)
    y0 = (int(row)-1) * 256
    y1 = y0 + 256

    outDir = "assets/tiles/" + curSceneHash
    if not os.path.exists(outDir):
        os.mkdir(outDir)

    outputName = "{}_{}-{}_{}-{}.png".format(zoom, x0, x1, y0, y1)
    outputFilepath = os.path.join(outDir, outputName)
    sendFile = False
    if os.path.exists(outputFilepath):
        print "Sending cached tile:", outputFilepath
        sendFile = True
    else:
        cmdLineArgs = "glow --input={0} --output={1} --imageArea={2},{3},{4},{5} --verbose". \
            format(curSceneFile, outputFilepath, x0, x1, y0, y1)

        print "Calling glow:", cmdLineArgs

        sendFile = glow.run(cmdLineArgs.split()) == 0

    if sendFile:
        print "sending {} {}".format(outDir, outputName)
        return send_from_directory(outDir, outputName)

    abort(500)


@app.route('/glow/preview')
def preview():
    global curNoisePreviewDir
    set_scene()

    # query=urllib.unquote(request.query_string).decode('utf8') 
    # # print "request query:", query
    # print "request query (json):", json.dumps(query)

    if curNoisePreviewDir == "" or \
        not os.path.exists(curNoisePreviewDir):
        curNoisePreviewDir = tempfile.mkdtemp(prefix='glow-noise-')

    cmdLineArgs = "glow --input={0} --preview={1} --verbose".format(curSceneFile, curNoisePreviewDir)
    print "Calling glow:", cmdLineArgs

    ret = glow.run(cmdLineArgs.split())
    retVal = json.dumps({'glow':ret}), 500, {'ContentType':'application/json'}

    images = []
    if ret == 0:
        print "-" * 80
        for bmpFile in os.listdir(curNoisePreviewDir):
            f, e = os.path.splitext(bmpFile)

            bmpPath = curNoisePreviewDir + "/" + bmpFile
            pngPath = curNoisePreviewDir + "/" + f + ".png"
            print "{0} -> {1}".format(bmpPath, pngPath)
            Image.open(bmpPath).save(pngPath, "PNG")

            if False:
                bmp = Image.open(bmpFile)
                png = StringIO()
                bmp.save(png, format="PNG")
                png_b64 = StringIO()
                base64.encode(png, png_b64)
                images.append({
                    'name':f,
                    'image':png_b64.getvalue()
                })

            print "   bmp to png", f 


        print "-" * 80
        # retVal = json.dumps({'images':images}), 200, {'ContentType':'application/json'}

        retVal = json.dumps({'glow':0}), 200, {'ContentType':'application/json'}

    return retVal

@app.route('/glow/noise/<path:path>')
def static_noise_proxy(path):
    global curNoisePreviewDir
    if curNoisePreviewDir != "" and os.path.exists(curNoisePreviewDir):
        print "img", os.path.join(curNoisePreviewDir, path)
        return send_from_directory(curNoisePreviewDir, path)

    abort(500)


# TODO:1 these two handlers might not be necessary.
@app.route('/js/<path:path>')
def static_js_proxy(path):
    return app.send_static_file(os.path.join('js', path))

@app.route('/css/<path:path>')
def static_css_proxy(path):
    return app.send_static_file(os.path.join('css', path))

@app.route('/fonts/<path:path>')
def static_fonts_proxy(path):
    return app.send_static_file(os.path.join('fonts', path))

@app.route('/images/<path:path>')
def static_images_proxy(path):
    return app.send_static_file(os.path.join('images', path))

@app.route('/scenes/<path:path>')
def static_scene_proxy(path):
    return app.send_static_file(os.path.join('scenes', path))


if __name__ == "__main__":
    app.debug = True
    app.run(port=5001)