#!/usr/bin/env python

from flask import Flask
import os

app = Flask(__name__, static_folder='assets')

@app.route('/')
def root():
    return app.send_static_file('glow.html')    

@app.route('/js/<path:path>')
def static_js_proxy(path):
    # send_static_file will guess the correct MIME type
    return app.send_static_file(os.path.join('js', path))

@app.route('/css/<path:path>')
def static_css_proxy(path):
    # send_static_file will guess the correct MIME type
    return app.send_static_file(os.path.join('css', path))

if __name__ == "__main__":
    app.debug = True
    app.run(port=5001)