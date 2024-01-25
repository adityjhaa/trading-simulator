from flask import Blueprint

auth = Blueprint('auth', __name__)

@auth.route('/register')
def register():
    return "<p>register</p>"

@auth.route('/logout')
def logout():
    return '<p>logout</p>'

@auth.route('login')
def login():
    return "<p>login</p>"