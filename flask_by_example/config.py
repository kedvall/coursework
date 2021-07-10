import os
basedir = os.path.abspath(os.path.dirname(__file__))
print("basedir:", basedir)

class Config(object):
    DEBUG = False
    TESTING = False
    CSRF_ENABLED = True
    SECRET_KEY = 'This-is-my-super-secret-key'
    SQLALCHEMY_DATABASE_URI = None

class ProductionConfig(Config):
    DEBUG = False

class StagingConfig(Config):
    DEVELOPMENT = True
    DEBUG = True

class DevelopmentConfig(Config):
    DEVELOPMENT = True
    DEBUG = True

class TestingConfig(Config):
    TESTING = True
