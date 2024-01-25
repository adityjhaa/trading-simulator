# thus file handles the frontend that is the home page
from flask import Blueprint

views = Blueprint('views', __name__)

@views.route('/')
def home():
    return "<h1>Hello World</h1>"