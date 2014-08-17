from bottle import route, run, static_file

@route('/hello')
def hello():
    return "Hello World!"

@route('/img')
def send_image():
    return static_file('dad-face1.png', root='/Users/tim/Documents/csuf/cpsc-599/img', mimetype='image/png')

run(host='localhost', port=8080)