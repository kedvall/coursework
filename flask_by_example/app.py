from flask import Flask
from flask_sqlalchemy import SQLAlchemy
import os


flask_app = Flask(__name__)

flask_config = os.getenv('APP_SETTINGS')
db_config = os.getenv('DATABASE_URI')

if flask_config is None:
    print("- Warning: No APP_SETTING env var found, defaulting to 'config.DevelopmentConfig'")
    flask_config = 'config.DevelopmentConfig'
if db_config is None:
    print("- Warning: No DATABASE_URI env var found, defaulting to 'postgresql:///wordcount_dev'")
    db_config = 'postgresql:///wordcount_dev'

print("Name:", __name__)
print("Flask config:", flask_config)
print("Database config:", db_config)


flask_app.config.from_object(flask_config)
flask_app.config['SQLALCHEMY_TRACK_MODIFICATIONS'] = False
db = SQLAlchemy(flask_app)

# from models import Result

@flask_app.route('/')
def hello():
    return "Hello World!"

@flask_app.route('/<name>')
def hello_name(name):
    return "Hello {}!".format(name)

if __name__ == '__main__':
    flask_app.run()
