#!/usr/bin/env python

from flask import Flask
import os

app = Flask(__name__, static_folder='assets')

@app.route('/')
def root():
    return app.send_static_file('glow.html')    

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