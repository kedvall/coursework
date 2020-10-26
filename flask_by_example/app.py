import os
from flask import Flask


flask_app = Flask(__name__)
print("Name:", __name__)

flask_config = os.getenv('APP_SETTINGS')
if flask_config is None:
    print("- Warning: No APP_SETTING env var found, defaulting to config.DevelopmentConfig")
    flask_config = 'config.DevelopmentConfig'
print("Config:", flask_config)

flask_app.config.from_object(flask_config)

@flask_app.route('/')
def hello():
    return "Hello World!"

@flask_app.route('/<name>')
def hello_name(name):
    return "Hello {}!".format(name)

if __name__ == '__main__':
    flask_app.run()
