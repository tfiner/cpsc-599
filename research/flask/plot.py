from flask import Flask, send_file
from PIL import Image
from StringIO import StringIO

app = Flask(__name__)

@app.route("/")
def hello():
    return "Hello World!"


page = """
<html>
    <body>
    Hey    
    </body>    
</html>
"""


@app.route("/test")
def test():
    return page


def serve_pil_image(pil_img):
    img_io = StringIO()
    pil_img.save(img_io, 'JPEG', quality=70)
    img_io.seek(0)
    return send_file(img_io, mimetype='image/jpeg')


@app.route('/image')
def serve_img():
    img = Image.new("RGB", (512, 512), "red")
    return serve_pil_image(img)


if __name__ == "__main__":
    app.debug = True
    app.run(port=5002)

