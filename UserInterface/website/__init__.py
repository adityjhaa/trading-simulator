from flask import Flask

def create_app():
    app = Flask(__name__)
    app.config['SECRET_KEY'] = "Starlord_Love_hurts_but_42_doesn't"
    
    return app