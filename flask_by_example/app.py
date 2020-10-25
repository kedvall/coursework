from flask import Flask
flask_app = Flask(__name__)

print("Name:", __name__)

@flask_app.route('/')
def hello():
    return "Hello World!"

@flask_app.route('/<name>')
def hello_name(name):
    return "Hello {}!".format(name)

if __name__ == '__main__':
    flask_app.run()
